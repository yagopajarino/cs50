import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session, url_for
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash
import datetime

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True


# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.route("/")
@login_required
def index():
    user_id = session["user_id"]
    data = db.execute("SELECT symbol, SUM(shares) FROM operations WHERE user_id = ? GROUP BY symbol;", user_id)
    cash = db.execute("SELECT cash FROM users WHERE id = ?", user_id)
    cash = cash[0]["cash"]
    port_value = 0
    tickers = []
    for x in data:
        a = {}
        a["symbol"] = x["symbol"]
        a["shares"] = x["SUM(shares)"]
        a["price"] = lookup(a["symbol"])["price"]
        a["f_price"] = usd(a["price"])
        a["name"] = lookup(a["symbol"])["name"]
        a["mkt_value"] = usd(a["price"] * a["shares"])
        port_value += a["price"] * a["shares"]
        tickers.append(a)
    return render_template("/index.html", cash = usd(cash), tickers = tickers, balance = usd(port_value + cash))


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "GET":
        return render_template("/buy.html", data = {"price":0}, shares = 0, cash = 0)
    else:
        symbol = request.form.get("symbol").lower()
        shares = request.form.get("shares")
        if not symbol or not shares:
            return apology("Input error")
        try:
            shares = int(shares)
        except:
            return apology("Invalid amount of shares", code = 400)
        data = lookup(symbol)
        if not data:
            return apology("Ticker not found", code = 400)
        if shares < 1:
            return apology("Shares must be non negative", code = 400)
        price = data["price"]
        amount = price * shares
        user_id =  session["user_id"]
        date = datetime.datetime.now()
        user_cash = db.execute("SELECT cash FROM users WHERE id = ?", user_id)
        user_cash = user_cash[0]["cash"]
        if user_cash < amount:
            return apology("you don't have enought money")
        db.execute("INSERT INTO operations (user_id, date, operation, symbol, shares, proce, amount) VALUES (?, ?, 'buy', ?, ?, ?, ?)", user_id, date, symbol, shares, price, amount)
        new_cash = user_cash - amount
        db.execute("UPDATE users SET cash = ? WHERE id = ?", new_cash, user_id)
        return redirect(url_for("index"))


@app.route("/get_price", methods=["GET","POST"])
@login_required
def price():
    if request.method == "GET":
        return redirect(url_for("buy"))
    else:
        symbol = request.form.get("symbol")
        shares = request.form.get("shares")
        if not symbol:
            return apology("Input error")
        if not shares:
            shares = 1
        try:
            data = lookup(symbol)
            user_id = session["user_id"]
            user_cash = db.execute("SELECT cash FROM users WHERE id = ?", user_id)
            user_cash = user_cash[0]["cash"]
            return render_template("/buy.html", data = data, shares=shares, cash=user_cash)
        except:
            return redirect(url_for("buy"))



@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    user_id = session["user_id"]
    operations = db.execute("SELECT * FROM operations WHERE user_id = ?;", user_id)
    return render_template("/history.html", operations = operations)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "GET":
        return render_template("/quote.html")
    else:
        symbol = request.form.get("symbol")
        if not symbol:
            return apology("Input ticker", code=400)
        data = lookup(symbol)
        if not data:
            return apology("Incorrect ticker", code=400)
        else:
            data["price"] = usd(data["price"])
            return render_template("quoted.html", data=data, symbol=symbol.upper())



@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    if request.method == "GET":
        return render_template("/register.html")
    else:
        username = request.form.get("username")
        password = request.form.get("password")
        if not username:
            return apology("Insert name")
        users = db.execute("SELECT * FROM users WHERE username = ?;", username)
        if len(users) > 0:
            return apology("Username already exists")
        if not password:
            return apology("Insert password")
        if password != request.form.get("confirmation") or not password:
            return apology("password error")
        db.execute("INSERT INTO users (username, hash) VALUES (?, ?)", username, generate_password_hash(password))
        return render_template("/login.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    if request.method == "GET":
        user_id = session["user_id"]
        stocks = db.execute("SELECT symbol FROM operations WHERE user_id = ? GROUP BY symbol HAVING SUM(amount) > 0;", user_id)
        return render_template("/sell.html", stocks=stocks)
    else:
        symbol = request.form.get("symbol").lower()
        shares = request.form.get("shares")
        if not shares:
            return apology("insert shares", code = 400)
        if symbol == "symbol":
            return apology("Invalid ticker", code = 400)
        try:
            shares = int(shares)
        except:
            return apology("Invalid amount of shares", code = 400)
        if shares < 1:
            return apology("Shares must be => 1", code = 400)
        user_id = session["user_id"]
        avalible = db.execute("SELECT SUM(shares) FROM operations WHERE (user_id = ? AND symbol = ?)", user_id, symbol)[0]["SUM(shares)"]
        if shares > avalible:
            return apology (str("Cant sell more than {} shares").format(avalible))
        date = datetime.datetime.now()
        price = lookup(symbol)["price"]
        amount = price * shares
        db.execute("INSERT INTO operations (user_id, date, operation, symbol, shares, proce, amount) VALUES (?, ?, 'sell', ?, ?, ?, ?)", user_id, date, symbol, -shares, -price, -amount)
        cash = db.execute("SELECT cash FROM users WHERE id = ?", user_id)[0]["cash"]
        new_cash = cash + amount
        db.execute("UPDATE users SET cash = ?", new_cash)
        return redirect(url_for("index"))


def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
