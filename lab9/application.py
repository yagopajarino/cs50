import os

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///birthdays.db")

@app.route("/", methods=["GET", "POST"])
def index():
    if request.method == "POST":

        # TODO: Add the user's entry into the database
        data = request.form
        db.execute("INSERT INTO birthdays (name, month, day) VALUES (?,?,?);", data["name"], data["month"], data["day"])
        return redirect("/")

    else:

        # TODO: Display the entries in the database on index.html
        data = db.execute("SELECT name, month, day FROM birthdays;")
        return render_template("index.html", data=data)


