// src/components/LandingPage.js
import React from 'react';
import { Container, Row, Col, Button, Navbar, Nav, Card } from 'react-bootstrap';
import TableSoccerHeader from './TableSoccerHeader';
import 'bootstrap/dist/css/bootstrap.min.css';



const LandingPage = () => {
  return (
    <div>
      {/* Header */}
      <TableSoccerHeader />

      {/* Hero Section */}
      <div className="hero-section text-white d-flex align-items-center">

       <Container className="text-center">
          <h1 className="display-4 text-black">Welcome to Table Soccer</h1>
          <p className="lead text-black">Manage your teams, games, and championships all in one place!</p>
          <Button variant="primary" size="lg" href="/dashboard">View Dashboard</Button>
        </Container>
      </div>

      {/* Features Section */}
      <Container id="features" className="my-4">
        <h2 className="text-center mb-4">Features</h2>
        <Row>

          <Col md={4} className="d-flex align-items-stretch">
            <Card className="custom-card text-center my-2">
              <Card.Body>
                <Card.Title>Players</Card.Title>
                <Card.Text>View player statistics and manage player profiles.</Card.Text>
                <Button variant="primary" href="/players">Manage</Button>
              </Card.Body>
            </Card>
          </Col>

          <Col md={4} className="d-flex align-items-stretch">
            <Card className="custom-card text-center my-2">
              <Card.Body>
                <Card.Title>Teams</Card.Title>
                <Card.Text>Create and organize teams with ease.</Card.Text>
                <Button variant="primary" href="/teams">Manage</Button>
              </Card.Body>
            </Card>
          </Col>

          <Col md={4} className="d-flex align-items-stretch">
            <Card className="custom-card text-center my-2">
              <Card.Body>
                <Card.Title>Games</Card.Title>
                <Card.Text>Track and manage all your games easily.</Card.Text>
                <Button variant="primary" href="/games">Manage</Button>
              </Card.Body>
            </Card>
          </Col>

        </Row>
      </Container>

      {/* About Section */}
      <Container id="about" className="my-5">
        <Row className="justify-content-center">
          <Col md={8} className="text-center">
            <h2>About Our App</h2>
            <p>This table soccer management app allows you to track every aspect of your games, players, and championships. Built with simplicity and usability in mind, it's perfect for enthusiasts and organizers alike.</p>
          </Col>
        </Row>
      </Container>

      {/* Contact Section */}
      <Container id="contact" className="my-5 text-center">
        <h2>Contact Us</h2>
        <p>If you have any questions, feel free to reach out to us!</p>
        <Button variant="primary" href="mailto:support@tablesoccerapp.com">Email Us</Button>
      </Container>

      {/* Footer */}
      <footer className="bg-dark text-white text-center py-4 mt-5">
        <Container>
          <Row>
            <Col>
              <p>&copy; {new Date().getFullYear()} Table Soccer App. All rights reserved.</p>
            </Col>
          </Row>
        </Container>
      </footer>
    </div>
  );
};

export default LandingPage;
