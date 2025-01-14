# CODING CHALLENGE: table_football

Check the document docs/challenge.pdf to see the specs.
References: https://www.tablesoccer.org/

# RUNNING USING DOCKER
* CLEAN: sudo docker-compose down -v
* BUILD: sudo docker-compose build
* RUN ( not in  background): sudo docker-compose up
* RUN ( in  background): sudo docker-compose up -d

### Then you can open your favorite browser
* http://localhost:3000 ( main site)
* http://localhost:5000/api-docs ( api doc )

# RUNNING LOCALLY
### ( I assume that node >14 is installed )

## POSTGRESQL
Make sure that postgresql is locally running, up and running! Create the database using the script db/create_db.sql 

## API
```
cd api
npm install ( run once )
node server.js
```

## MAINAPP
```
cd apps/main
npm install (run once )
npm start
```

## Reuse the url above to see the sites. Happy gaming
