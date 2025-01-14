// server.js
const express = require('express');
const cors = require('cors');
const swaggerJsDoc = require('swagger-jsdoc');
const swaggerUi = require('swagger-ui-express');
const playersRoutes = require('./PlayersRoutes');
const teamsRoutes = require('./TeamsRoutes');
const gamesRoutes = require('./GamesRoutes');
const statsRoutes = require('./StatsRoutes');

require('dotenv').config();

const app = express();
const PORT = process.env.PORT || 5000;

app.use(cors());
app.use(express.json());

// Swagger configuration options
const swaggerOptions = {
  definition: {
    openapi: '3.0.0',
    info: {
      title: 'Table Soccer API',
      version: '1.0.0',
      description: 'API documentation for the Table Soccer application',
    },
    servers: [
      {
        url: `http://localhost:${PORT}`,
      },
    ],
  },
  // Paths to files containing OpenAPI documentation
  //'/opt/api/*Routes.js' is an horrible trick because I install the api in /opt
  apis: ['./PlayersRoutes.js', './TeamsRoutes.js', './GamesRoutes.js', './StatsRoutes.js', '/opt/api/*Routes.js'], // Paths to files containing OpenAPI documentation
};

// Generate Swagger docs
const swaggerDocs = swaggerJsDoc(swaggerOptions);
app.use('/api-docs', swaggerUi.serve, swaggerUi.setup(swaggerDocs));

// Add API routes
app.use(playersRoutes);
app.use(teamsRoutes);
app.use(gamesRoutes);
app.use(statsRoutes);

// Start the server
app.listen(PORT, () => {
  console.log(`Server running on http://localhost:${PORT}`);
  console.log(`API docs available at http://localhost:${PORT}/api-docs`);
});

