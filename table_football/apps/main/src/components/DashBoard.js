// src/components/Dashboard.js
import React, { useState, useEffect } from 'react';
import { Container, Table, Button, Alert, Pagination } from 'react-bootstrap';
import TableSoccerHeader from './TableSoccerHeader';
import { useNavigate } from 'react-router-dom';
import axios from 'axios';

const Dashboard = () => {
  const [stats, setStats] = useState([]);
  const [currentPage, setCurrentPage] = useState(1);
  const [sortOrder, setSortOrder] = useState('asc');
  const statsPerPage = 10;
  const navigate = useNavigate();

  useEffect(() => {
    fetchStatistics();
  }, []);

  const fetchStatistics = async () => {
    try {
      const response = await axios.get('http://localhost:5000/api/statistics');
      const sortedData = response.data.sort((a, b) => parseFloat(b.winRatio) - parseFloat(a.winRatio));
      setStats(sortedData);
    } catch (error) {
      console.error('Error fetching statistics:', error);
    }
  };

  const toggleSort = () => {
    const newOrder = sortOrder === 'asc' ? 'desc' : 'asc';
    const sortedStats = [...stats].sort((a, b) => {
      return newOrder === 'asc'
        ? parseFloat(a.winRatio) - parseFloat(b.winRatio)
        : parseFloat(b.winRatio) - parseFloat(a.winRatio);
    });
    setStats(sortedStats);
    setSortOrder(newOrder);
    setCurrentPage(1);
  };

  const indexOfLastStat = currentPage * statsPerPage;
  const indexOfFirstStat = indexOfLastStat - statsPerPage;
  const currentStats = stats.slice(indexOfFirstStat, indexOfLastStat);

  const paginate = (pageNumber) => setCurrentPage(pageNumber);

  return (
    <div>
      <TableSoccerHeader />
      <Container className="my-5">
        <h2>Dashboard</h2>

        {stats.length === 0 ? (
          <>
            <Alert variant="info" className="text-center">No statistics available to display.</Alert>
            <div className="text-center">
              <Button variant="primary" onClick={() => navigate('/games')}>
                Go to Games
              </Button>
            </div>
          </>
        ) : (
          <>
            <Table striped bordered hover>
              <thead>
                <tr>
                  <th>Type</th>
                  <th>Name</th>
                  <th>Games Played</th>
                  <th>Wins</th>
                  <th>Losses</th>
                  <th>Draws</th>
                  <th onClick={toggleSort} style={{ cursor: 'pointer' }}>
                    Win Ratio {sortOrder === 'asc' ? '▲' : '▼'}
                  </th>
                  <th>GF</th>
                  <th>GA</th>
                  <th>GD</th>
                </tr>
              </thead>
              <tbody>
                {currentStats.map((stat, index) => (
                  <tr key={index}>
                    <td
                      style={{
                        backgroundColor: stat.type === 'team' ? '#58e0e5' : '#00ff40',
                        color: 'white',
                        textAlign: 'center',
                        fontWeight: 'bold'
                      }}
                    >
                      {stat.type === 'team' ? 'Team' : 'Player'}
                    </td>
                    <td>{stat.name}</td>
                    <td>{stat.gamesPlayed}</td>
                    <td>{stat.wins}</td>
                    <td>{stat.losses}</td>
                    <td>{stat.draws}</td>
                    <td>{stat.winRatio}</td>
                    <td>{stat.goalsFor}</td>
                    <td>{stat.goalsAgainst}</td>
                    <td>{stat.goalDifference}</td>
                  </tr>
                ))}
              </tbody>
            </Table>

            <Pagination className="justify-content-center">
              {Array.from({ length: Math.ceil(stats.length / statsPerPage) }, (_, index) => (
                <Pagination.Item
                  key={index + 1}
                  active={index + 1 === currentPage}
                  onClick={() => paginate(index + 1)}
                >
                  {index + 1}
                </Pagination.Item>
              ))}
            </Pagination>
          </>
        )}
      </Container>
    </div>
  );
};

export default Dashboard;

