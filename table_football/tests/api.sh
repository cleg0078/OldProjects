#LIST PLAYERS
curl http://localhost:5000/api/players

#ADD PLAYER
curl -X POST -H "Content-Type: application/json" -d '{"name": "John Doe"}' http://localhost:5000/api/players

#UPDATE PLAYER 
curl -X PUT -H "Content-Type: application/json" -d '{"name": "Jane Doen"}' http://localhost:5000/api/players/1
 
#DELETE PLAYER 
curl -X DELETE http://localhost:5000/api/players/1




