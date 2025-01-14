// src/components/TeamsManagement.js
import React, { useState, useEffect } from 'react';
import { Container, Table, Button, Alert, Toast, Form, Pagination } from 'react-bootstrap';
import axios from 'axios';
import TeamsForm from './TeamsForm';
import TableSoccerHeader from './TableSoccerHeader';

const TeamsManagement = () => {
  const [players, setPlayers] = useState([]);
  const [teams, setTeams] = useState([]);
  const [filteredTeams, setFilteredTeams] = useState([]);
  const [message, setMessage] = useState('');
  const [error, setError] = useState('');
  const [showToast, setShowToast] = useState(false);
  const [showModal, setShowModal] = useState(false);
  const [searchTerm, setSearchTerm] = useState('');
  const [currentPage, setCurrentPage] = useState(1);
  const teamsPerPage = 5;

  useEffect(() => {
    fetchPlayers();
    fetchTeams();
  }, []);

  // Fetch players from the API
  const fetchPlayers = async () => {
    try {
      const response = await axios.get('http://localhost:5000/api/players');
      setPlayers(response.data);
    } catch (error) {
      console.error('Failed to fetch players:', error);
      setError('Could not fetch players. Please try again later.');
      setShowToast(true);
    }
  };

  // Fetch teams from the API
  const fetchTeams = async () => {
    try {
      const response = await axios.get('http://localhost:5000/api/teams');
      setTeams(response.data);
      setFilteredTeams(response.data);
    } catch (error) {
      console.error('Failed to fetch teams:', error);
      setError('Could not fetch teams. Please try again later.');
      setShowToast(true);
    }
  };

  // Handle team creation submission
  const handleCreateTeam = async ({ teamName, player1, player2 }) => {
    if (!teamName || !player1 || (player1 === player2 && player2)) {
      setError('Invalid team data. Please ensure that Player 1 and Player 2 are different.');
      setShowToast(true);
      return;
    }

    try {
      const response = await axios.post('http://localhost:5000/api/teams', {
        name: teamName,
        player1_id: player1,
        player2_id: player2 || null,
      });

      setMessage(`Team "${response.data.name}" created successfully!`);
      setTimeout(() => setMessage(''), 4000);

      fetchTeams();
      setTeams([...teams, response.data]);
      setFilteredTeams([...filteredTeams, response.data]);
    } catch (error) {
      const errorMessage = error.response?.data?.error || 'Could not create team. Please try again later.';
      setError(errorMessage);
      setShowToast(true);
    }
  };

  // Handle search input change
  const handleSearch = (e) => {
    setSearchTerm(e.target.value);
    const term = e.target.value.toLowerCase();
    const filtered = teams.filter((team) =>
      team.name.toLowerCase().includes(term)
    );
    setFilteredTeams(filtered);
    setCurrentPage(1); // Reset to first page on search
  };

  // Pagination calculations
  const indexOfLastTeam = currentPage * teamsPerPage;
  const indexOfFirstTeam = indexOfLastTeam - teamsPerPage;
  const currentTeams = filteredTeams.slice(indexOfFirstTeam, indexOfLastTeam);

  const paginate = (pageNumber) => setCurrentPage(pageNumber);

  return (
    <div>
    <TableSoccerHeader/>
    <Container className="my-5">
      <h2>Teams Management</h2>

      {/* Success Message */}
      {message && <Alert variant="success">{message}</Alert>}

      {/* Search Bar */}
      <Form.Control
        type="text"
        placeholder="Search teams by name"
        value={searchTerm}
        onChange={handleSearch}
        className="mb-3"
      />

      {/* Button to Open Modal */}
      <Button variant="primary" onClick={() => setShowModal(true)} className="mb-4">
        Create Team
      </Button>

      {/* TeamsForm Modal */}
      <TeamsForm
        show={showModal}
        handleClose={() => setShowModal(false)}
        onSubmit={handleCreateTeam}
        players={players}
      />

      {/* Display Teams Table */}
      <h3>Teams List</h3>
      {filteredTeams.length === 0 ? (
        <p>No teams have been created yet.</p>
      ) : (
        <>
          <Table striped bordered hover>
            <thead>
              <tr>
                <th>#</th>
                <th>Team Name</th>
                <th>Player 1</th>
                <th>Player 2</th>
              </tr>
            </thead>
            <tbody>
              {currentTeams.map((team, index) => (
                <tr key={team.id}>
                  <td>{indexOfFirstTeam + index + 1}</td>
                  <td>{team.name}</td>
                  <td>{team.player1_name || 'N/A'}</td>
                  <td>{team.player2_name || 'N/A'}</td>
                </tr>
              ))}
            </tbody>
          </Table>

          {/* Pagination Controls */}
          <Pagination className="justify-content-center mt-3">
            {Array.from({ length: Math.ceil(filteredTeams.length / teamsPerPage) }, (_, index) => (
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

      {/* Error Toast */}
      <Toast
        onClose={() => setShowToast(false)}
        show={showToast}
        delay={3000}
        autohide
        style={{ position: 'fixed', bottom: 20, right: 20 }}
      >
        <Toast.Header>
          <strong className="me-auto">Error</strong>
        </Toast.Header>
        <Toast.Body>{error}</Toast.Body>
      </Toast>
    </Container>
    </div>
  );
};

export default TeamsManagement;

