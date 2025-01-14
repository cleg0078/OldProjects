// src/components/PlayerStatsModal.js
import React, { useState, useEffect } from 'react';
import { Modal, Table, Button, Spinner, Pagination } from 'react-bootstrap';
import axios from 'axios';

const PlayerStatsModal = ({ show, onHide, playerId, playerName }) => {
  const [games, setGames] = useState([]);
  const [loading, setLoading] = useState(false);
  const [currentPage, setCurrentPage] = useState(1);
  const gamesPerPage = 10; // Number of games per page

  useEffect(() => {
    const fetchPlayerGames = async () => {
      setLoading(true);
      try {
        const response = await axios.get(`http://localhost:5000/api/players/${playerId}/games`);
        setGames(response.data);
      } catch (error) {
        console.error('Error fetching player games:', error);
      } finally {
        setLoading(false);
      }
    };

    if (show && playerId) {
      fetchPlayerGames();
    }
  }, [show, playerId]);

  // Calculate indices for current page
  const indexOfLastGame = currentPage * gamesPerPage;
  const indexOfFirstGame = indexOfLastGame - gamesPerPage;
  const currentGames = games.slice(indexOfFirstGame, indexOfLastGame);
  const totalPages = Math.ceil(games.length / gamesPerPage);

  const handlePageChange = (pageNumber) => setCurrentPage(pageNumber);

  return (
    <Modal show={show} onHide={onHide} centered>
      <Modal.Header closeButton>
        <Modal.Title>Games Played by {playerName}</Modal.Title>
      </Modal.Header>
      <Modal.Body>
        {loading ? (
          <Spinner animation="border" />
        ) : games.length > 0 ? (
          <>
            <Table striped bordered hover>
              <thead>
                <tr>
                  <th>#</th>
                  <th>Opponent</th>
                  <th>Score</th>
                  <th>Result</th>
                </tr>
              </thead>
              <tbody>
                {currentGames.map((game, index) => (
                  <tr key={game.id}>
                    <td>{indexOfFirstGame + index + 1}</td>
                    <td>{game.opponent_name}</td>
                    <td>{game.team1_score} - {game.team2_score}</td>
                    <td>{game.result}</td>
                  </tr>
                ))}
              </tbody>
            </Table>
            <Pagination className="justify-content-center mt-3">
              {[...Array(totalPages)].map((_, pageIndex) => (
                <Pagination.Item
                  key={pageIndex + 1}
                  active={pageIndex + 1 === currentPage}
                  onClick={() => handlePageChange(pageIndex + 1)}
                >
                  {pageIndex + 1}
                </Pagination.Item>
              ))}
            </Pagination>
          </>
        ) : (
          <p>No games played yet.</p>
        )}
      </Modal.Body>
      <Modal.Footer>
        <Button variant="secondary" onClick={onHide}>
          Close
        </Button>
      </Modal.Footer>
    </Modal>
  );
};

export default PlayerStatsModal;

