const { Pool } = require('pg');

const pool = new Pool({
  host: process.env.DB_HOST,         // should be 'db' per docker-compose
  port: process.env.DB_PORT || 5432,
  user: process.env.DB_USER || 'postgres',
  password: process.env.DB_PASSWORD || '',
  database: process.env.DB_NAME || 'table_soccer',
});

module.exports = pool;


/*
const { Pool } = require('pg');
require('dotenv').config();

const pool = new Pool({
  connectionString: process.env.DATABASE_URL,
});

pool.on('connect', () => {
  console.log('Connected to the PostgreSQL database');
});

module.exports = {
  query: (text, params) => pool.query(text, params),
};
*/
