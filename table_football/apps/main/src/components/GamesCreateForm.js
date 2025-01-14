// src/components/GamesCreateForm.js
import React, { useState } from 'react';
import { Button, Modal, Form, Alert } from 'react-bootstrap';

const GamesCreateForm = ({ show, onHide, onCreate, teams }) => {
  const [selectedTeam1, setSelectedTeam1] = useState('');
  const [selectedTeam2, setSelectedTeam2] = useState('');
  const [message, setMessage] = useState('');

  const handleSubmit = (e) => {
    e.preventDefault();
    if (!selectedTeam1 || !selectedTeam2 || selectedTeam1 === selectedTeam2) {
      setMessage('Please select two different teams.');
      return;
    }
    onCreate(selectedTeam1, selectedTeam2);
    resetForm();
  };

  const resetForm = () => {
    setSelectedTeam1('');
    setSelectedTeam2('');
    setMessage('');
    onHide();
  };

  return (
    <Modal show={show} onHide={resetForm} centered>
      <Modal.Header closeButton>
        <Modal.Title>Start New Game</Modal.Title>
      </Modal.Header>
      <Modal.Body>
        {message && <Alert variant="danger">{message}</Alert>}
        <Form onSubmit={handleSubmit}>
          <Form.Group controlId="team1" className="mb-3">
            <Form.Label>Select Team 1</Form.Label>
            <Form.Select value={selectedTeam1} onChange={(e) => setSelectedTeam1(e.target.value)} required>
              <option value="">Select team</option>
              {teams.map((team) => (
                <option key={team.id} value={team.id}>
                  {team.name}
                </option>
              ))}
            </Form.Select>
          </Form.Group>
          <Form.Group controlId="team2" className="mb-3">
            <Form.Label>Select Team 2</Form.Label>
            <Form.Select value={selectedTeam2} onChange={(e) => setSelectedTeam2(e.target.value)} required>
              <option value="">Select team</option>
              {teams.map((team) => (
                <option key={team.id} value={team.id}>
                  {team.name}
                </option>
              ))}
            </Form.Select>
          </Form.Group>
          <Button type="submit" variant="primary">
            Start Game
          </Button>
        </Form>
      </Modal.Body>
    </Modal>
  );
};

export default GamesCreateForm;

