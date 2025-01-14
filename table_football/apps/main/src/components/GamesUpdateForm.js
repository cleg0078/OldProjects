// src/components/GameUpdateForm.js
import React, { useState } from 'react';
import { Modal, Button, Form, Alert } from 'react-bootstrap';

const GamesUpdateForm = ({ show, onHide, game, onUpdate }) => {
  const [team1Score, setTeam1Score] = useState(game.team1_score || '');
  const [team2Score, setTeam2Score] = useState(game.team2_score || '');
  const [message, setMessage] = useState('');

  const handleSubmit = (e) => {
    e.preventDefault();

    // Validate scores to ensure they are non-negative
    if (team1Score < 0 || team2Score < 0) {
      setMessage('Scores cannot be negative. Please enter valid scores.');
      return;
    }

    if (team1Score === '' || team2Score === '') {
      setMessage('Please enter scores for both teams.');
      return;
    }

    onUpdate(game.id, team1Score, team2Score);
    resetForm();
  };

  const resetForm = () => {
    setTeam1Score('');
    setTeam2Score('');
    setMessage('');
    onHide();
  };

  return (
    <Modal show={show} onHide={resetForm} centered>
      <Modal.Header closeButton>
        <Modal.Title>Update Game Score</Modal.Title>
      </Modal.Header>
      <Modal.Body>
        {message && <Alert variant="danger">{message}</Alert>}
        <Form onSubmit={handleSubmit}>
          <Form.Group controlId="team1Score" className="mb-3">
            <Form.Label>{game.team1_name} Score</Form.Label>
            <Form.Control
              type="number"
              value={team1Score}
              onChange={(e) => setTeam1Score(e.target.value)}
              min="0"
              required
            />
          </Form.Group>
          <Form.Group controlId="team2Score" className="mb-3">
            <Form.Label>{game.team2_name} Score</Form.Label>
            <Form.Control
              type="number"
              value={team2Score}
              onChange={(e) => setTeam2Score(e.target.value)}
              min="0"
              required
            />
          </Form.Group>
          <Button type="submit" variant="primary">
            Update Score
          </Button>
        </Form>
      </Modal.Body>
    </Modal>
  );
};

export default GamesUpdateForm;

