// src/components/TableSoccerHeader.js
import React from 'react';
import { Navbar, Nav, Container, Button } from 'react-bootstrap';
import { useNavigate } from 'react-router-dom';
import { FaHome } from 'react-icons/fa'; // Import home icon from React Icons

const TableSoccerHeader = () => {
  const navigate = useNavigate();

  return (
	   <Navbar bg="dark" variant="dark" expand="lg" sticky="top">
      <Container>
        <Navbar.Brand href="/">Table Soccer App</Navbar.Brand>
        <Navbar.Toggle aria-controls="basic-navbar-nav" />
        <Navbar.Collapse id="basic-navbar-nav">
          <Nav className="me-auto">
          </Nav>

          {/* Return to Landing Page Button */}
          <Button
            variant="outline-light"
            onClick={() => navigate('/')}
            className="d-flex align-items-center"
          >
            <FaHome className="me-2" /> {/* Icon */}
            Home
          </Button>
        </Navbar.Collapse>
      </Container>
    </Navbar>
  );
};

export default TableSoccerHeader;
