// src/components/TeamsForm.js
import React, { useState } from 'react';
import { Modal, Form, Button } from 'react-bootstrap';

const TeamsForm = ({ show, handleClose, onSubmit, players }) => {
  const [teamName, setTeamName] = useState('');
  const [player1, setPlayer1] = useState('');
  const [player2, setPlayer2] = useState('');

  // Handle form submission and reset form fields
  const handleFormSubmit = (e) => {
    e.preventDefault();
    onSubmit({ teamName, player1, player2 });
    setTeamName('');
    setPlayer1('');
    setPlayer2('');
    handleClose(); // Close modal on submit
  };

  return (
    <Modal show={show} onHide={handleClose} centered>
      <Modal.Header closeButton>
        <Modal.Title>Create Team</Modal.Title>
      </Modal.Header>
      <Modal.Body>
        <Form onSubmit={handleFormSubmit}>
          <Form.Group controlId="teamName" className="mb-3">
            <Form.Label>Team Name</Form.Label>
            <Form.Control
              type="text"
              placeholder="Enter team name"
              value={teamName}
              onChange={(e) => setTeamName(e.target.value)}
              required
            />
          </Form.Group>

          <Form.Group controlId="player1" className="mb-3">
            <Form.Label>Player 1</Form.Label>
            <Form.Select
              value={player1}
              onChange={(e) => setPlayer1(e.target.value)}
              required
            >
              <option value="">Select player</option>
              {players.map((player) => (
                <option key={player.id} value={player.id}>
                  {player.name}
                </option>
              ))}
            </Form.Select>
          </Form.Group>

          <Form.Group controlId="player2" className="mb-3">
            <Form.Label>Player 2 (Optional)</Form.Label>
            <Form.Select
              value={player2}
              onChange={(e) => setPlayer2(e.target.value)}
            >
              <option value="">Select player</option>
              {players.map((player) => (
                <option key={player.id} value={player.id}>
                  {player.name}
                </option>
              ))}
            </Form.Select>
          </Form.Group>

          <Button variant="primary" type="submit">
            Create Team
          </Button>
        </Form>
      </Modal.Body>
    </Modal>
  );
};

export default TeamsForm;

