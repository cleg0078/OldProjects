// TeamsRoutes.js

const express = require('express');
const db = require('./db'); // Import the database connection
const router = express.Router();

/**
 * @swagger
 * tags:
 *   name: Teams
 *   description: API endpoints for managing teams
 */

/**
 * @swagger
 * /api/teams:
 *   post:
 *     summary: Create a new team
 *     tags: [Teams]
 *     requestBody:
 *       required: true
 *       content:
 *         application/json:
 *           schema:
 *             type: object
 *             properties:
 *               name:
 *                 type: string
 *                 description: Name of the team.
 *                 example: "The Champions"
 *               player1_id:
 *                 type: integer
 *                 description: ID of the first player.
 *                 example: 1
 *               player2_id:
 *                 type: integer
 *                 description: ID of the second player (optional).
 *                 example: 2
 *     responses:
 *       201:
 *         description: Team created successfully
 *       400:
 *         description: Invalid input or team name not unique
 *       500:
 *         description: Database error
 */
router.post('/api/teams', async (req, res) => {
  const { name, player1_id, player2_id } = req.body;

  if (!name || !player1_id) {
    return res.status(400).json({ error: 'Team name and player1_id are required.' });
  }
  if (player2_id && player1_id === player2_id) {
    return res.status(400).json({ error: 'Player1 and Player2 must be different.' });
  }

  try {
    const result = await db.query(
      'INSERT INTO teams (name, player1_id, player2_id) VALUES ($1, $2, $3) RETURNING *',
      [name, player1_id, player2_id]
    );
    res.status(201).json(result.rows[0]);
  } catch (error) {
    if (error.code === '23505') {
      return res.status(400).json({ error: 'Team name must be unique' });
    }
    console.error('Error creating team:', error);
    res.status(500).json({ error: 'Database error' });
  }
});

/**
 * @swagger
 * /api/teams:
 *   get:
 *     summary: Get all teams
 *     tags: [Teams]
 *     responses:
 *       200:
 *         description: A list of teams
 *         content:
 *           application/json:
 *             schema:
 *               type: array
 *               items:
 *                 type: object
 *                 properties:
 *                   id:
 *                     type: integer
 *                   name:
 *                     type: string
 *                   player1_name:
 *                     type: string
 *                   player2_name:
 *                     type: string
 *       500:
 *         description: Database error
 */
router.get('/api/teams', async (req, res) => {
  try {
    const result = await db.query(`
      SELECT t.id, t.name, p1.name AS player1_name, p2.name AS player2_name
      FROM teams t
      LEFT JOIN players p1 ON t.player1_id = p1.id
      LEFT JOIN players p2 ON t.player2_id = p2.id
      ORDER BY t.id
    `);
    res.json(result.rows);
  } catch (error) {
    console.error('Error fetching teams:', error);
    res.status(500).json({ error: 'Database error' });
  }
});

/**
 * @swagger
 * /api/teams/{id}:
 *   get:
 *     summary: Get a team by ID
 *     tags: [Teams]
 *     parameters:
 *       - in: path
 *         name: id
 *         required: true
 *         schema:
 *           type: integer
 *     responses:
 *       200:
 *         description: A team object
 *         content:
 *           application/json:
 *             schema:
 *               type: object
 *               properties:
 *                 id:
 *                   type: integer
 *                 name:
 *                   type: string
 *                 player1_name:
 *                   type: string
 *                 player2_name:
 *                   type: string
 *       404:
 *         description: Team not found
 *       500:
 *         description: Database error
 */
router.get('/api/teams/:id', async (req, res) => {
  const { id } = req.params;
  try {
    const result = await db.query(`
      SELECT t.id, t.name, p1.name AS player1_name, p2.name AS player2_name
      FROM teams t
      LEFT JOIN players p1 ON t.player1_id = p1.id
      LEFT JOIN players p2 ON t.player2_id = p2.id
      WHERE t.id = $1
    `, [id]);

    if (result.rows.length === 0) {
      return res.status(404).json({ error: 'Team not found' });
    }
    res.json(result.rows[0]);
  } catch (error) {
    console.error('Error fetching team:', error);
    res.status(500).json({ error: 'Database error' });
  }
});

/**
 * @swagger
 * /api/teams/{id}:
 *   put:
 *     summary: Update a team by ID
 *     tags: [Teams]
 *     parameters:
 *       - in: path
 *         name: id
 *         required: true
 *         schema:
 *           type: integer
 *     requestBody:
 *       required: true
 *       content:
 *         application/json:
 *           schema:
 *             type: object
 *             properties:
 *               name:
 *                 type: string
 *               player1_id:
 *                 type: integer
 *               player2_id:
 *                 type: integer
 *     responses:
 *       200:
 *         description: Team updated successfully
 *       400:
 *         description: Invalid input or team name not unique
 *       404:
 *         description: Team not found
 *       500:
 *         description: Database error
 */
router.put('/api/teams/:id', async (req, res) => {
  const { id } = req.params;
  const { name, player1_id, player2_id } = req.body;

  if (!name || !player1_id) {
    return res.status(400).json({ error: 'Team name and player1_id are required.' });
  }
  if (player2_id && player1_id === player2_id) {
    return res.status(400).json({ error: 'Player1 and Player2 must be different.' });
  }

  try {
    const result = await db.query(
      'UPDATE teams SET name = $1, player1_id = $2, player2_id = $3 WHERE id = $4 RETURNING *',
      [name, player1_id, player2_id, id]
    );

    if (result.rows.length === 0) {
      return res.status(404).json({ error: 'Team not found' });
    }
    res.json(result.rows[0]);
  } catch (error) {
    if (error.code === '23505') {
      return res.status(400).json({ error: 'Team name must be unique' });
    }
    console.error('Error updating team:', error);
    res.status(500).json({ error: 'Database error' });
  }
});

/**
 * @swagger
 * /api/teams/{id}:
 *   delete:
 *     summary: Delete a team by ID
 *     tags: [Teams]
 *     parameters:
 *       - in: path
 *         name: id
 *         required: true
 *         schema:
 *           type: integer
 *     responses:
 *       200:
 *         description: Team deleted successfully
 *       404:
 *         description: Team not found
 *       500:
 *         description: Database error
 */
router.delete('/api/teams/:id', async (req, res) => {
  const { id } = req.params;
  try {
    const result = await db.query('DELETE FROM teams WHERE id = $1 RETURNING *', [id]);

    if (result.rows.length === 0) {
      return res.status(404).json({ error: 'Team not found' });
    }
    res.json({ message: 'Team deleted successfully' });
  } catch (error) {
    console.error('Error deleting team:', error);
    res.status(500).json({ error: 'Database error' });
  }
});

module.exports = router;

