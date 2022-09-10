-- Keep a log of any SQL queries you execute as you solve the mystery.

-- Looking at crime scene reports
SELECT description FROM crime_scene_reports WHERE year = 2020 AND month = 07 AND day >= 28;

-- Theft of the CS50 duck took place at 10:15am at the Chamberlin Street courthouse.
-- Interviews were conducted today with three witnesses who were present at the time — each of their interview transcripts mentions the courthouse.

-- Looking at interviews
SELECT transcript FROM interviews WHERE year = 2020 AND month = 07 and day >= 28;

-- Sometime within ten minutes of the theft, I saw the thief get into a car in the courthouse parking lot and drive away.
-- If you have security footage from the courthouse parking lot, you might want to look for cars that left the parking lot in that time frame.

-- I don't know the thief's name, but it was someone I recognized. Earlier this morning, before I arrived at the courthouse, I was walking by the ATM on Fifer Street and saw the thief there withdrawing some money.

-- As the thief was leaving the courthouse, they called someone who talked to them for less than a minute. In the call, I heard the thief say that they were planning to take the earliest flight out of Fiftyville tomorrow.
-- The thief then asked the person on the other end of the phone to purchase the flight ticket.

-- Looking for bank accounts that withdraw money on that thay on that ATM
SELECT account_number FROM atm_transactions WHERE year = 2020 AND month = 07 AND day = 28 AND atm_location = "Fifer Street" AND transaction_type = "withdraw";

-- Getting phone numbers of calls that day with duration less than a minute
SELECT caller FROM phone_calls WHERE year = 2020 AND month = 07 AND day = 28 AND duration < 60;

-- License plate
SELECT license_plate FROM courthouse_security_logs WHERE year = 2020 AND month = 07 AND day = 28 AND hour = 10 AND minute > 15 AND minute < 25;

-- Crossing phone number and account number with people data
SELECT people.id FROM people
JOIN bank_accounts ON bank_accounts.person_id = people.id
WHERE people.phone_number IN (SELECT caller FROM phone_calls WHERE year = 2020 AND month = 07 AND day = 28 AND duration < 60)
        AND people.id IN (SELECT person_id FROM bank_accounts WHERE account_number IN (SELECT account_number FROM atm_transactions WHERE year = 2020 AND month = 07 AND day = 28 AND atm_location = "Fifer Street" AND transaction_type = "withdraw")
        AND people.license_plate IN (SELECT license_plate FROM courthouse_security_logs WHERE year = 2020 AND month = 07 AND day = 28 AND hour = 10 AND minute > 15 AND minute < 25));

-- 686048
-- 514354

-- That took flight on 2020/07/29
SELECT flights.id FROM flights
JOIN airports ON airports.id = flights.origin_airport_id
WHERE airports.city = "Fiftyville" AND flights.year = 2020 AND flights.month = 07 AND flights.day = 29 ORDER BY flights.hour, flights.minute LIMIT 1;

-- Passengers on those flights
SELECT passport_number FROM passengers
WHERE flight_id IN (SELECT flights.id FROM flights
                    JOIN airports ON airports.id = flights.origin_airport_id
                    WHERE airports.city = "Fiftyville" AND flights.year = 2020 AND flights.month = 07 AND flights.day = 29);

-- Passport and id
SELECT id FROM people
WHERE passport_number IN (SELECT passport_number FROM passengers
                            WHERE flight_id IN (SELECT flights.id FROM flights
                            JOIN airports ON airports.id = flights.origin_airport_id
                            WHERE airports.city = "Fiftyville" AND flights.year = 2020 AND flights.month = 07 AND flights.day = 29));

-- Getting thief ID
SELECT * FROM people
JOIN bank_accounts ON bank_accounts.person_id = people.id
WHERE people.phone_number IN (SELECT caller FROM phone_calls WHERE year = 2020 AND month = 07 AND day = 28 AND duration < 60)
        AND people.id IN (SELECT person_id FROM bank_accounts WHERE account_number IN (SELECT account_number FROM atm_transactions WHERE year = 2020 AND month = 07 AND day = 28 AND atm_location = "Fifer Street" AND transaction_type = "withdraw"))
        AND people.license_plate IN (SELECT license_plate FROM courthouse_security_logs WHERE year = 2020 AND month = 07 AND day = 28 AND hour = 10 AND minute > 15 AND minute < 25)
        AND people.id IN (SELECT id FROM people
                            WHERE passport_number IN (SELECT passport_number FROM passengers
                                                     WHERE flight_id IN (SELECT flights.id FROM flights
                                                                        JOIN airports ON airports.id = flights.origin_airport_id
                                                                        WHERE airports.city = "Fiftyville" AND flights.year = 2020 AND flights.month = 07 AND flights.day = 29 ORDER BY flights.hour, flights.minute LIMIT 1)));

-- id | name | phone_number | passport_number | license_plate | account_number | person_id | creation_year
-- 686048 | Ernest | (367) 555-5533 | 5773159633 | 94KL13X | 49610011 | 686048 | 2010

-- Destination
SELECT airports.city FROM airports
JOIN flights ON flights.destination_airport_id = airports.id
WHERE flights.id IN (SELECT flights.id FROM flights
                        JOIN airports ON airports.id = flights.origin_airport_id
                        WHERE airports.city = "Fiftyville" AND flights.year = 2020 AND flights.month = 07 AND flights.day = 29 ORDER BY flights.hour, flights.minute LIMIT 1);

-- Accomplice
SELECT * FROM people
WHERE phone_number IN (SELECT receiver FROM phone_calls WHERE year = 2020 AND month = 07 AND day = 28 AND duration < 60 AND caller = "(367) 555-5533");
