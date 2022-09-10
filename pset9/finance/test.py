from cs50 import SQL

db = SQL("sqlite:///finance.db")

user_id = 1
symbol = "ypf"
avalible = db.execute("SELECT SUM(shares) FROM operations WHERE (user_id = ? AND symbol = ?)", user_id, symbol)[0]["SUM(shares)"]
print(type(avalible))
"""
if shares > avalible:
    return apology (str("Cant sell more than {} shares").format(avalible))"""