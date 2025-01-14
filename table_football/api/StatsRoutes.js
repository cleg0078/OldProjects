// StatsRoutes.js
const express = require('express');
const router = express.Router();
const db = require('./db');

/**
 * @swagger
 * tags:
 *   name: Statistics
 *   description: API endpoints for retrieving statistics on teams and players
 */

/**
 * @swagger
 * /api/statistics:
 *   get:
 *     summary: Get combined statistics for teams and players
 *     tags: [Statistics]
 *     description: Retrieve combined statistics for teams and players, including games played, wins, losses, draws, and goal differences.
 *     responses:
 *       200:
 *         description: A list of combined statistics for teams and players
 *         content:
 *           application/json:
 *             schema:
 *               type: array
 *               items:
 *                 type: object
 *                 properties:
 *                   name:
 *                     type: string
 *                   type:
 *                     type: string
 *                   gamesPlayed:
 *                     type: integer
 *                   wins:
 *                     type: integer
 *                   losses:
 *                     type: integer
 *                   draws:
 *                     type: integer
 *                   winRatio:
 *                     type: string
 *                   goalsFor:
 *                     type: integer
 *                   goalsAgainst:
 *                     type: integer
 *                   goalDifference:
 *                     type: integer
 *       500:
 *         description: Failed to fetch statistics due to a server error
 */
router.get('/api/statistics', async (req, res) => {
  try {
    // Team statistics query, with draws included
    const teamResult = await db.query(`
      SELECT 
        teams.name AS name,
        COUNT(games.id) AS games_played,
        SUM(CASE WHEN (games.team1_id = teams.id AND games.team1_score > games.team2_score) OR 
                      (games.team2_id = teams.id AND games.team2_score > games.team1_score) THEN 1 ELSE 0 END) AS wins,
        SUM(CASE WHEN (games.team1_id = teams.id AND games.team1_score < games.team2_score) OR 
                      (games.team2_id = teams.id AND games.team2_score < games.team1_score) THEN 1 ELSE 0 END) AS losses,
        SUM(CASE WHEN games.team1_score = games.team2_score THEN 1 ELSE 0 END) AS draws,
        SUM(CASE WHEN games.team1_id = teams.id THEN games.team1_score ELSE games.team2_score END) AS goals_for,
        SUM(CASE WHEN games.team1_id = teams.id THEN games.team2_score ELSE games.team1_score END) AS goals_against,
        'team' AS type
      FROM teams
      LEFT JOIN games ON (games.team1_id = teams.id OR games.team2_id = teams.id) AND games.completed = true
      GROUP BY teams.id
    `);

    // Player statistics query, with draws included
    const playerResult = await db.query(`
      SELECT
        players.name AS name,
        COUNT(games.id) AS games_played,
        SUM(CASE WHEN (games.team1_id = teams.id AND games.team1_score > games.team2_score) OR
                      (games.team2_id = teams.id AND games.team2_score > games.team1_score) THEN 1 ELSE 0 END) AS wins,
        SUM(CASE WHEN (games.team1_id = teams.id AND games.team1_score < games.team2_score) OR
                      (games.team2_id = teams.id AND games.team2_score < games.team1_score) THEN 1 ELSE 0 END) AS losses,
        SUM(CASE WHEN games.team1_score = games.team2_score THEN 1 ELSE 0 END) AS draws,
        SUM(CASE WHEN games.team1_id = teams.id THEN games.team1_score ELSE games.team2_score END) AS goals_for,
        SUM(CASE WHEN games.team1_id = teams.id THEN games.team2_score ELSE games.team1_score END) AS goals_against,
        'player' AS type
      FROM players
      JOIN teams ON players.id = teams.player1_id OR players.id = teams.player2_id
      LEFT JOIN games ON (games.team1_id = teams.id OR games.team2_id = teams.id) AND games.completed = true
      GROUP BY players.id
    `);

    const combinedStats = [...teamResult.rows, ...playerResult.rows].map((stat) => ({
      name: stat.name,
      type: stat.type,
      gamesPlayed: parseInt(stat.games_played, 10),
      wins: parseInt(stat.wins, 10),
      losses: parseInt(stat.losses, 10),
      draws: parseInt(stat.draws, 10),
      winRatio: (stat.wins / stat.games_played).toFixed(2),
      goalsFor: parseInt(stat.goals_for, 10),
      goalsAgainst: parseInt(stat.goals_against, 10),
      goalDifference: stat.goals_for - stat.goals_against,
    }));

    res.json(combinedStats);
  } catch (error) {
    console.error('Error fetching combined statistics:', error);
    res.status(500).json({ error: 'Failed to fetch statistics' });
  }
});

module.exports = router;

