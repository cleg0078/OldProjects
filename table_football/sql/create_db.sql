--CREATE DATABASE table_soccer;

CREATE TABLE players (
  id SERIAL PRIMARY KEY,
  name TEXT NOT NULL UNIQUE
);

CREATE TABLE teams (
    id SERIAL PRIMARY KEY,
    name VARCHAR(100) NOT NULL UNIQUE,
    player1_id INT NOT NULL,      -- Must have at least one player
    player2_id INT,               -- Optional second player
    CHECK (player1_id IS DISTINCT FROM player2_id), -- Ensure players are unique
    FOREIGN KEY (player1_id) REFERENCES players(id), -- Reference to players table
    FOREIGN KEY (player2_id) REFERENCES players(id) -- Reference to players table
);

CREATE TABLE games (
  id SERIAL PRIMARY KEY,            -- Unique identifier for each game
  team1_id INT NOT NULL,             -- Foreign key to the first team
  team2_id INT NOT NULL,             -- Foreign key to the second team
  team1_score INT DEFAULT 0,         -- Score for the first team, default is 0
  team2_score INT DEFAULT 0,         -- Score for the second team, default is 0
  completed BOOLEAN DEFAULT false,   -- Game completion status, default is false
  created_at TIMESTAMP DEFAULT NOW(),-- Timestamp when the game was created

  -- Foreign key constraints
  FOREIGN KEY (team1_id) REFERENCES teams(id) ON DELETE CASCADE,
  FOREIGN KEY (team2_id) REFERENCES teams(id) ON DELETE CASCADE,
  
  -- Ensure team1 and team2 are different
  CONSTRAINT unique_teams CHECK (team1_id <> team2_id)
);
