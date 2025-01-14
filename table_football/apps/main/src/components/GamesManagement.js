// src/components/GamesManagement.js
import React, { useState, useEffect } from 'react';
import { Alert, Container, Row, Col, Table, Button, Pagination } from 'react-bootstrap';
import axios from 'axios';
import TableSoccerHeader from './TableSoccerHeader';
import GamesCreateForm from './GamesCreateForm';
import GamesUpdateForm from './GamesUpdateForm';

const GamesManagement = () => {
  const [teams, setTeams] = useState([]);
  const [pastGames, setPastGames] = useState([]);
  const [message, setMessage] = useState('');
  const [showCreateModal, setShowCreateModal] = useState(false);
  const [showUpdateModal, setShowUpdateModal] = useState(false);
  const [currentGame, setCurrentGame] = useState(null);
  const [currentPage, setCurrentPage] = useState(1);
  const gamesPerPage = 10;

  useEffect(() => {
    fetchTeams();
    fetchPastGames();
  }, []);

  const fetchTeams = async () => {
    try {
      const response = await axios.get('http://localhost:5000/api/teams');
      setTeams(response.data);
    } catch (error) {
      console.error('Failed to fetch teams:', error);
    }
  };

  const fetchPastGames = async () => {
    try {
      const response = await axios.get('http://localhost:5000/api/games');
      setPastGames(response.data);
    } catch (error) {
      console.error('Failed to fetch past games:', error);
    }
  };

  const handleCreateNewGame = async (team1Id, team2Id) => {
    try {
      await axios.post('http://localhost:5000/api/games', {
        team1_id: team1Id,
        team2_id: team2Id,
        team1_score: null,
        team2_score: null,
        completed: false,
      });
      setMessage(`New game started between ${getTeamName(team1Id)} and ${getTeamName(team2Id)}.`);
      fetchPastGames();
    } catch (error) {
      console.error('Failed to create new game:', error);
    }
  };

  const handleUpdateScore = async (gameId, team1Score, team2Score) => {
    try {
      await axios.put(`http://localhost:5000/api/games/${gameId}`, {
        team1_score: team1Score,
        team2_score: team2Score,
        completed: true,
      });
      setMessage('Game score updated successfully');
      fetchPastGames();
    } catch (error) {
      console.error('Failed to update game score:', error);
    }
  };

  const getTeamName = (teamId) => teams.find((team) => team.id === teamId)?.name || '';

  const indexOfLastGame = currentPage * gamesPerPage;
  const indexOfFirstGame = indexOfLastGame - gamesPerPage;
  const currentGames = pastGames.slice(indexOfFirstGame, indexOfLastGame);
  const paginate = (pageNumber) => setCurrentPage(pageNumber);

  return (
    <div>
      <TableSoccerHeader />
      <Container className="my-5">
        <h2>Game Management</h2>

        {message && <Alert variant="success">{message}</Alert>}

        {/* Button to Open Create Game Modal */}
        <Row className="mb-4">
          <Col>
            <Button variant="primary" onClick={() => setShowCreateModal(true)}>
              Start New Game
            </Button>
          </Col>
        </Row>

        <h4>Past Games</h4>
        {pastGames.length === 0 ? (
          <Alert variant="info" className="text-center">
            No games available.
          </Alert>
        ) : (
          <>
            <Table striped bordered hover>
              <thead>
                <tr>
                  <th>#</th>
                  <th>Team 1</th>
                  <th>Team 2</th>
                  <th>Score</th>
                  <th>Actions</th>
                </tr>
              </thead>
              <tbody>
                {currentGames.map((game, index) => (
                  <tr key={game.id}>
                    <td>{indexOfFirstGame + index + 1}</td>
                    <td>{getTeamName(game.team1_id)}</td>
                    <td>{getTeamName(game.team2_id)}</td>
                    <td>{game.completed ? `${game.team1_score} - ${game.team2_score}` : 'Incomplete'}</td>
                    <td>
                      {!game.completed && (
                        <Button
                          variant="warning"
                          size="sm"
                          onClick={() => {
                            setCurrentGame({
                              ...game,
                              team1_name: getTeamName(game.team1_id),
                              team2_name: getTeamName(game.team2_id),
                            });
                            setShowUpdateModal(true);
                          }}
                        >
                          Update Score
                        </Button>
                      )}
                    </td>
                  </tr>
                ))}
              </tbody>
            </Table>

            <Pagination className="justify-content-center mt-3">
              {Array.from({ length: Math.ceil(pastGames.length / gamesPerPage) }, (_, index) => (
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

      {/* Game Creation Modal */}
      <GamesCreateForm
        show={showCreateModal}
        onHide={() => setShowCreateModal(false)}
        onCreate={handleCreateNewGame}
        teams={teams}
      />

      {/* Game Update Score Modal */}
      {currentGame && (
        <GamesUpdateForm
          show={showUpdateModal}
          onHide={() => setShowUpdateModal(false)}
          game={currentGame}
          onUpdate={handleUpdateScore}
        />
      )}
    </div>
  );
};

export default GamesManagement;

