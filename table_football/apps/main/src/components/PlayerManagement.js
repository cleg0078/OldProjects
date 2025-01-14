// src/components/PlayerManagement.js
import React, { useState, useEffect } from 'react';
import { Container, Row, Col, Table, Button, Pagination, Form, Toast } from 'react-bootstrap';
import { useNavigate } from 'react-router-dom';
import axios from 'axios';
import PlayerForm from './PlayerForm';
import PlayerStatsModal from './PlayerStatsModal';
import TableSoccerHeader from './TableSoccerHeader';

const PlayerManagement = () => {
  const [players, setPlayers] = useState([]);
  const [showForm, setShowForm] = useState(false);
  const [selectedPlayer, setSelectedPlayer] = useState(null);
  const [showStatsModal, setShowStatsModal] = useState(false);
  const [currentPage, setCurrentPage] = useState(1);
  const [searchTerm, setSearchTerm] = useState('');
  const [showToast, setShowToast] = useState(false);
  const [toastMessage, setToastMessage] = useState('');
  const playersPerPage = 50;

  useEffect(() => {
    const fetchPlayers = async () => {
      try {
        const response = await axios.get('http://localhost:5000/api/players');
        setPlayers(response.data);
      } catch (error) {
        handleError(error, 'Failed to fetch players');
      }
    };

    fetchPlayers();
  }, []);

  const handleError = (error, defaultMessage) => {
    const message = error.response?.data?.error || defaultMessage;
    setToastMessage(message);
    setShowToast(true);
  };

  const handleShowGames = (player) => {
    setSelectedPlayer(player);
    setShowStatsModal(true);
  };

  const filteredPlayers = players.filter((player) =>
    player.name.toLowerCase().includes(searchTerm.toLowerCase())
  );

  const indexOfLastPlayer = currentPage * playersPerPage;
  const indexOfFirstPlayer = indexOfLastPlayer - playersPerPage;
  const currentPlayers = filteredPlayers.slice(indexOfFirstPlayer, indexOfLastPlayer);

  const handleSavePlayer = async (playerData) => {
    try {
      const response = await axios.post('http://localhost:5000/api/players', playerData);
      setPlayers([...players, response.data]);
    } catch (error) {
      handleError(error, 'Failed to add player');
    }
    setShowForm(false);
  };

  const handleDeletePlayer = async (id) => {
    if (window.confirm('Are you sure you want to delete this player?')) {
      try {
        await axios.delete(`http://localhost:5000/api/players/${id}`);
        setPlayers(players.filter((player) => player.id !== id));
        setToastMessage('Player deleted successfully');
      } catch (error) {
        const errorMessage = error.response?.data?.error || 'Failed to delete player';
        setToastMessage(errorMessage);
      }
      setShowToast(true);
    }
  };

  const paginate = (pageNumber) => setCurrentPage(pageNumber);
  const totalPages = Math.ceil(filteredPlayers.length / playersPerPage);

  return (
    <div>
      <TableSoccerHeader />
      <Container className="my-5">
        <Row className="mb-4">
          <Col>
            <h2>Player Management</h2>
            <Button variant="primary" onClick={() => { setShowForm(true); }}>
              Add Player
            </Button>
          </Col>
        </Row>

        <Row className="mb-4">
          <Col md={4}>
            <Form.Control
              type="text"
              placeholder="Search by player name"
              value={searchTerm}
              onChange={(e) => setSearchTerm(e.target.value)}
            />
          </Col>
        </Row>

        {filteredPlayers.length === 0 ? (
          <Row>
            <Col className="text-center">
              <p>No players found.</p>
            </Col>
          </Row>
        ) : (
          <>
            <Table striped bordered hover>
              <thead>
                <tr>
                  <th>#</th>
                  <th>Name</th>
                  <th>Actions</th>
                </tr>
              </thead>
              <tbody>
                {currentPlayers.map((player, index) => (
                  <tr key={player.id}>
                    <td>{indexOfFirstPlayer + index + 1}</td>
                    <td>{player.name}</td>
                    <td>
                      <Button
                        variant="danger"
                        size="sm"
                        onClick={() => handleDeletePlayer(player.id)}
                      >
                        Delete
                      </Button>{' '}
                      <Button
                        variant="info"
                        size="sm"
                        onClick={() => handleShowGames(player)}
                      >
                        Show Games
                      </Button>
                    </td>
                  </tr>
                ))}
              </tbody>
            </Table>

            <Pagination className="justify-content-center mt-4">
              {[...Array(totalPages)].map((_, pageIndex) => (
                <Pagination.Item
                  key={pageIndex + 1}
                  active={pageIndex + 1 === currentPage}
                  onClick={() => paginate(pageIndex + 1)}
                >
                  {pageIndex + 1}
                </Pagination.Item>
              ))}
            </Pagination>
          </>
        )}

        <Toast
          onClose={() => setShowToast(false)}
          show={showToast}
          delay={3000}
          autohide
          style={{ position: 'fixed', bottom: 20, right: 20 }}
        >
          <Toast.Header>
            <strong className="me-auto">Message</strong>
          </Toast.Header>
          <Toast.Body>{toastMessage}</Toast.Body>
        </Toast>

        <PlayerForm
          show={showForm}
          onHide={() => setShowForm(false)}
          onSave={handleSavePlayer}
          initialData={selectedPlayer}
        />

        {selectedPlayer && (
          <PlayerStatsModal
            show={showStatsModal}
            onHide={() => setShowStatsModal(false)}
            playerId={selectedPlayer.id}
            playerName={selectedPlayer.name}
          />
        )}
      </Container>
    </div>
  );
};

export default PlayerManagement;

