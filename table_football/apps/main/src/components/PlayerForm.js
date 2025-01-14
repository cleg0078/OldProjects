// src/components/PlayerForm.js
import React, { useState } from 'react';
import { Modal, Form, Button } from 'react-bootstrap';

const PlayerForm = ({ show, onHide, onSave }) => {
  const [name, setName] = useState('');

  const handleSubmit = (e) => {
    e.preventDefault();
    onSave({ name });
    setName('');
    onHide();
  };

  return (
    <Modal show={show} onHide={onHide}>
      <Modal.Header closeButton>
        <Modal.Title>Add New Player</Modal.Title>
      </Modal.Header>
      <Modal.Body>
        <Form onSubmit={handleSubmit}>
          <Form.Group controlId="playerName">
            <Form.Label>Player Name</Form.Label>
            <Form.Control
              type="text"
              placeholder="Enter player name"
              value={name}
              onChange={(e) => setName(e.target.value)}
              required
            />
          </Form.Group>
          <Button variant="primary" type="submit" className="mt-4">
            Add Player
          </Button>
        </Form>
      </Modal.Body>
    </Modal>
  );
};

export default PlayerForm;

