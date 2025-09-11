For MySQL connection (Assignment 8)
In case of error connecting to database with username and password , try:

CREATE USER 'username'@'localhost' IDENTIFIED BY 'password';
GRANT ALL PRIVILEGES ON database_name.* TO 'user'@'localhost';
FLUSH PRIVILEGES;
