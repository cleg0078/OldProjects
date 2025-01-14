// PlayersRoutes.js
const express = require('express');
const db = require('./db');

const router = express.Router();

/**
 * @swagger
 * tags:
 *   name: Players
 *   description: API endpoints for managing players
 */

/**
 * @swagger
 * /api/players:
 *   get:
 *     summary: Get all players
 *     tags: [Players]
 *     responses:
 *       200:
 *         description: A list of players
 *         content:
 *           application/json:
 *             schema:
 *               type: array
 *               items:
 *                 type: object
 *                 properties:
 *                   id:
 *                     type: integer
 *                     example: 1
 *                   name:
 *                     type: string
 *                     example: John Doe
 */
router.get('/api/players', async (req, res) => {
  try {
    const result = await db.query('SELECT * FROM players ORDER BY id');
    res.json(result.rows);
  } catch (error) {
    console.error('Error fetching players:', error);
    res.status(500).json({ error: 'Database error' });
  }
});

/**
 * @swagger
 * /api/players/{id}:
 *   get:
 *     summary: Get a single player by ID
 *     tags: [Players]
 *     parameters:
 *       - in: path
 *         name: id
 *         required: true
 *         schema:
 *           type: integer
 *         description: The player's ID
 *     responses:
 *       200:
 *         description: A player object
 *         content:
 *           application/json:
 *             schema:
 *               type: object
 *               properties:
 *                 id:
 *                   type: integer
 *                   example: 1
 *                 name:
 *                   type: string
 *                   example: John Doe
 *       404:
 *         description: Player not found
 */
router.get('/api/players/:id', async (req, res) => {
  try {
    const { id } = req.params;
    const result = await db.query('SELECT * FROM players WHERE id = $1', [id]);
    if (result.rows.length === 0) {
      return res.status(404).json({ error: 'Player not found' });
    }
    res.json(result.rows[0]);
  } catch (error) {
    console.error('Error fetching player:', error);
    res.status(500).json({ error: 'Database error' });
  }
});

/**
 * @swagger
 * /api/players:
 *   post:
 *     summary: Add a new player
 *     tags: [Players]
 *     requestBody:
 *       required: true
 *       content:
 *         application/json:
 *           schema:
 *             type: object
 *             properties:
 *               name:
 *                 type: string
 *                 example: John Doe
 *     responses:
 *       201:
 *         description: The created player
 *         content:
 *           application/json:
 *             schema:
 *               type: object
 *               properties:
 *                 id:
 *                   type: integer
 *                   example: 1
 *                 name:
 *                   type: string
 *                   example: John Doe
 *       400:
 *         description: Player name must be unique
 */
router.post('/api/players', async (req, res) => {
  try {
    const { name } = req.body;

    const existingPlayer = await db.query('SELECT * FROM players WHERE name = $1', [name]);
    if (existingPlayer.rows.length > 0) {
      return res.status(400).json({ error: 'Player name must be unique' });
    }
    const result = await db.query(
      'INSERT INTO players (name) VALUES ($1) RETURNING *',
      [name]
    );
    res.status(201).json(result.rows[0]);
  } catch (error) {
    console.error('Error adding player:', error);
    res.status(500).json({ error: 'Database error' });
  }
});

/**
 * @swagger
 * /api/players/{id}:
 *   put:
 *     summary: Update a player's name by ID
 *     tags: [Players]
 *     parameters:
 *       - in: path
 *         name: id
 *         required: true
 *         schema:
 *           type: integer
 *         description: The player's ID
 *     requestBody:
 *       required: true
 *       content:
 *         application/json:
 *           schema:
 *             type: object
 *             properties:
 *               name:
 *                 type: string
 *                 example: Jane Doe
 *     responses:
 *       200:
 *         description: The updated player object
 *         content:
 *           application/json:
 *             schema:
 *               type: object
 *               properties:
 *                 id:
 *                   type: integer
 *                   example: 1
 *                 name:
 *                   type: string
 *                   example: Jane Doe
 *       404:
 *         description: Player not found
 */
router.put('/api/players/:id', async (req, res) => {
  try {
    const { id } = req.params;
    const { name } = req.body;
    const result = await db.query(
      'UPDATE players SET name = $1 WHERE id = $2 RETURNING *',
      [name, id]
    );
    if (result.rows.length === 0) {
      return res.status(404).json({ error: 'Player not found' });
    }
    res.json(result.rows[0]);
  } catch (error) {
    console.error('Error updating player:', error);
    res.status(500).json({ error: 'Database error' });
  }
});

/**
 * @swagger
 * /api/players/{id}:
 *   delete:
 *     summary: Delete a player by ID
 *     tags: [Players]
 *     parameters:
 *       - in: path
 *         name: id
 *         required: true
 *         schema:
 *           type: integer
 *         description: The player's ID
 *     responses:
 *       200:
 *         description: Player deleted successfully
 *         content:
 *           application/json:
 *             schema:
 *               type: object
 *               properties:
 *                 message:
 *                   type: string
 *                   example: Player deleted successfully
 *       400:
 *         description: Player is part of a team and cannot be deleted
 *       404:
 *         description: Player not found
 */
router.delete('/api/players/:id', async (req, res) => {
  const playerId = req.params.id;
  try {
    const teamCheck = await db.query(
      'SELECT * FROM teams WHERE player1_id = $1 OR player2_id = $1',
      [playerId]
    );

    if (teamCheck.rows.length > 0) {
      return res.status(400).json({ error: 'Player is part of a team and cannot be deleted.' });
    }

    await db.query('DELETE FROM players WHERE id = $1', [playerId]);
    res.status(200).json({ message: 'Player deleted successfully' });
  } catch (error) {
    console.error('Error deleting player:', error);
    res.status(500).json({ error: 'An error occurred while deleting the player.' });
  }
});


/**
 * @swagger
 * /api/players/{id}/games:
 *   get:
 *     summary: Get games played by a specific player
 *     tags: [Players]
 *     parameters:
 *       - in: path
 *         name: id
 *         required: true
 *         schema:
 *           type: integer
 *         description: The player's ID
 *     responses:
 *       200:
 *         description: A list of games played by the player
 *         content:
 *           application/json:
 *             schema:
 *               type: array
 *               items:
 *                 type: object
 *                 properties:
 *                   id:
 *                     type: integer
 *                     description: Game ID
 *                     example: 1
 *                   opponent_name:
 *                     type: string
 *                     description: Name of the opposing team or player
 *                     example: Team B
 *                   team1_score:
 *                     type: integer
 *                     description: Score of team1
 *                     example: 3
 *                   team2_score:
 *                     type: integer
 *                     description: Score of team2
 *                     example: 2
 *                   result:
 *                     type: string
 *                     description: Game result for the player
 *                     example: Win
 *       404:
 *         description: Player not found
 *       500:
 *         description: Database error
 */
router.get('/api/players/:id/games', async (req, res) => {
  const playerId = req.params.id;
  try {
    // Ensure player exists
    const playerCheck = await db.query('SELECT * FROM players WHERE id = $1', [playerId]);
    if (playerCheck.rows.length === 0) {
      return res.status(404).json({ error: 'Player not found' });
    }

    // Fetch games for the player
    const gamesResult = await db.query(`
      SELECT 
        g.id, 
        CASE 
          WHEN g.team1_id = t.id THEN t2.name 
          ELSE t1.name 
        END AS opponent_name,
        g.team1_score, 
        g.team2_score,
        CASE 
          WHEN (g.team1_id = t.id AND g.team1_score > g.team2_score) OR 
               (g.team2_id = t.id AND g.team2_score > g.team1_score) THEN 'Win'
          WHEN (g.team1_id = t.id AND g.team1_score < g.team2_score) OR 
               (g.team2_id = t.id AND g.team2_score < g.team1_score) THEN 'Loss'
          ELSE 'Draw'
        END AS result
      FROM games g
      JOIN teams t ON (t.player1_id = $1 OR t.player2_id = $1)
      JOIN teams t1 ON g.team1_id = t1.id
      JOIN teams t2 ON g.team2_id = t2.id
      WHERE g.team1_id = t.id OR g.team2_id = t.id
      ORDER BY g.id DESC
    `, [playerId]);

    res.json(gamesResult.rows);
  } catch (error) {
    console.error('Error fetching player games:', error);
    res.status(500).json({ error: 'Database error' });
  }
});


module.exports = router;

