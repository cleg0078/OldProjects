// routes/GamesRoutes.js

const express = require('express');
const router = express.Router();
const db = require('./db');

/**
 * @swagger
 * tags:
 *   name: Games
 *   description: API endpoints for managing games
 */

/**
 * @swagger
 * /api/games:
 *   post:
 *     summary: Create a new game
 *     tags: [Games]
 *     description: Create a new game with scores and completion status.
 *     requestBody:
 *       required: true
 *       content:
 *         application/json:
 *           schema:
 *             type: object
 *             properties:
 *               team1_id:
 *                 type: integer
 *                 description: ID of Team 1.
 *                 example: 1
 *               team2_id:
 *                 type: integer
 *                 description: ID of Team 2.
 *                 example: 2
 *               team1_score:
 *                 type: integer
 *                 description: Score of Team 1.
 *                 example: 0
 *               team2_score:
 *                 type: integer
 *                 description: Score of Team 2.
 *                 example: 0
 *               completed:
 *                 type: boolean
 *                 description: Game completion status.
 *                 example: false
 *     responses:
 *       201:
 *         description: Game created successfully
 *         content:
 *           application/json:
 *             schema:
 *               type: object
 *               properties:
 *                 id:
 *                   type: integer
 *                 team1_id:
 *                   type: integer
 *                 team2_id:
 *                   type: integer
 *                 team1_score:
 *                   type: integer
 *                 team2_score:
 *                   type: integer
 *                 completed:
 *                   type: boolean
 *       400:
 *         description: Validation error
 *       500:
 *         description: Server error
 */
router.post('/api/games', async (req, res) => {
  const { team1_id, team2_id, team1_score = 0, team2_score = 0, completed = false } = req.body;

  if (!team1_id || !team2_id || team1_id === team2_id) {
    return res.status(400).json({ error: 'Please select two different teams.' });
  }

  try {
    const newGame = await db.query(
      `INSERT INTO games (team1_id, team2_id, team1_score, team2_score, completed) 
       VALUES ($1, $2, $3, $4, $5) RETURNING *`,
      [team1_id, team2_id, team1_score, team2_score, completed]
    );
    res.status(201).json(newGame.rows[0]);
  } catch (error) {
    console.error('Error creating game:', error);
    res.status(500).json({ error: 'Failed to create game' });
  }
});

/**
 * @swagger
 * /api/games:
 *   get:
 *     summary: Get all games
 *     tags: [Games]
 *     description: Retrieve a list of all games, including team names and scores.
 *     responses:
 *       200:
 *         description: A list of games
 *         content:
 *           application/json:
 *             schema:
 *               type: array
 *               items:
 *                 type: object
 *                 properties:
 *                   id:
 *                     type: integer
 *                   team1_id:
 *                     type: integer
 *                   team2_id:
 *                     type: integer
 *                   team1_score:
 *                     type: integer
 *                   team2_score:
 *                     type: integer
 *                   completed:
 *                     type: boolean
 *                   team1_name:
 *                     type: string
 *                   team2_name:
 *                     type: string
 *       500:
 *         description: Server error
 */
router.get('/api/games', async (req, res) => {
  try {
    const games = await db.query(
      `SELECT g.id, g.team1_id, g.team2_id, g.team1_score, g.team2_score, g.completed,
              t1.name AS team1_name, t2.name AS team2_name
       FROM games g
       JOIN teams t1 ON g.team1_id = t1.id
       JOIN teams t2 ON g.team2_id = t2.id
       ORDER BY g.id DESC`
    );
    res.json(games.rows);
  } catch (error) {
    console.error('Error fetching games:', error);
    res.status(500).json({ error: 'Failed to fetch games' });
  }
});

/**
 * @swagger
 * /api/games/{id}:
 *   put:
 *     summary: Update game score and status
 *     tags: [Games]
 *     description: Update the scores of a game and mark it as completed.
 *     parameters:
 *       - in: path
 *         name: id
 *         required: true
 *         schema:
 *           type: integer
 *         description: The ID of the game to update.
 *     requestBody:
 *       required: true
 *       content:
 *         application/json:
 *           schema:
 *             type: object
 *             properties:
 *               team1_score:
 *                 type: integer
 *                 description: Updated score of Team 1.
 *                 example: 2
 *               team2_score:
 *                 type: integer
 *                 description: Updated score of Team 2.
 *                 example: 3
 *               completed:
 *                 type: boolean
 *                 description: Mark game as completed.
 *                 example: true
 *     responses:
 *       200:
 *         description: Game updated successfully
 *         content:
 *           application/json:
 *             schema:
 *               type: object
 *               properties:
 *                 id:
 *                   type: integer
 *                 team1_score:
 *                   type: integer
 *                 team2_score:
 *                   type: integer
 *                 completed:
 *                   type: boolean
 *       400:
 *         description: Validation error
 *       404:
 *         description: Game not found
 *       500:
 *         description: Server error
 */
router.put('/api/games/:id', async (req, res) => {
  const { id } = req.params;
  const { team1_score, team2_score, completed } = req.body;

  if (team1_score === undefined || team2_score === undefined) {
    return res.status(400).json({ error: 'Please provide scores for both teams.' });
  }

  try {
    const result = await db.query(
      'UPDATE games SET team1_score = $1, team2_score = $2, completed = $3 WHERE id = $4 RETURNING *',
      [team1_score, team2_score, completed, id]
    );

    if (result.rowCount === 0) {
      return res.status(404).json({ error: 'Game not found' });
    }

    res.status(200).json(result.rows[0]);
  } catch (error) {
    console.error('Error updating game:', error);
    res.status(500).json({ error: 'Failed to update game' });
  }
});

module.exports = router;

