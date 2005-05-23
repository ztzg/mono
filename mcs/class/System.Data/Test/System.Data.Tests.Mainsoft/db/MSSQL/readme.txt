in order to create the testing database, on SQLServer, run:
Run the scripts with a user wich have administrator permissions. (by default user:sa, password:sa).

osql -S <database> -U <username> -P <password> -i GHTDB.MSSQL.sql
osql -S <database> -U <username> -P <password> -i GHTDB.DATA.MSSQL.sql

for example
osql -S xp050 -U sa -P sa -i GHTDB.MSSQL.sql
