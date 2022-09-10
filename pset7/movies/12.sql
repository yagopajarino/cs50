SELECT movies.title FROM movies
WHERE movies.id IN (SELECT movies.id FROM movies
                    JOIN stars ON stars.movie_id = movies.id
                    JOIN people ON people.id = stars.person_id
                    WHERE people.name = "Johnny Depp")
        AND movies.id IN (SELECT movies.id FROM movies
                    JOIN stars ON stars.movie_id = movies.id
                    JOIN people ON people.id = stars.person_id
                    WHERE people.name = "Helena Bonham Carter");