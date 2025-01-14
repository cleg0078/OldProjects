import './App.css';
import React from 'react';
import { BrowserRouter as Router, Routes, Route } from 'react-router-dom';
import LandingPage from './components/LandingPage';
import PlayerManagement from './components/PlayerManagement';
import TeamManagement from './components/TeamManagement';
import GamesManagement from './components/GamesManagement';
import DashBoard from './components/DashBoard';

const App = () => {
  return (
    <Router>
      <Routes>
        <Route path="/" element={<LandingPage />} />
	<Route path="/players" element={<PlayerManagement />} />
        <Route path="/teams" element={<TeamManagement />} />
        <Route path="/games" element={<GamesManagement />} />
        <Route path="/dashboard" element={<DashBoard />} />
      </Routes>
    </Router>
  );
};

export default App;

