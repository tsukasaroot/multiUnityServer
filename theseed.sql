-- phpMyAdmin SQL Dump
-- version 5.0.2
-- https://www.phpmyadmin.net/
--
-- Hôte : 127.0.0.1
-- Généré le : Dim 24 jan. 2021 à 15:48
-- Version du serveur :  10.4.14-MariaDB
-- Version de PHP : 7.4.10

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
START TRANSACTION;
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Base de données : `theseed`
--

-- --------------------------------------------------------

--
-- Structure de la table `accounts`
--

CREATE TABLE `accounts` (
  `account_id` int(11) NOT NULL,
  `name` varchar(50) NOT NULL DEFAULT '0',
  `password` varchar(255) NOT NULL DEFAULT '',
  `email` varchar(50) NOT NULL DEFAULT '',
  `VIP` tinyint(1) NOT NULL DEFAULT 0
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Déchargement des données de la table `accounts`
--

INSERT INTO `accounts` (`account_id`, `name`, `password`, `email`, `VIP`) VALUES
(1, 'test', 'puissant', '', 0),
(2, 'testo', 'test', '', 0);

-- --------------------------------------------------------

--
-- Structure de la table `appearance`
--

CREATE TABLE `appearance` (
  `character_id` int(11) NOT NULL,
  `head` int(11) NOT NULL,
  `body` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

-- --------------------------------------------------------

--
-- Structure de la table `broker`
--

CREATE TABLE `broker` (
  `broker_id` double DEFAULT NULL,
  `item_id` double DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Structure de la table `currentplayerstats`
--

CREATE TABLE `currentplayerstats` (
  `user_id` int(20) NOT NULL,
  `hp` double NOT NULL,
  `mp` double NOT NULL,
  `attack` double NOT NULL,
  `critRate` double NOT NULL,
  `critP` double NOT NULL,
  `defense` double NOT NULL,
  `re` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

--
-- Déchargement des données de la table `currentplayerstats`
--

INSERT INTO `currentplayerstats` (`user_id`, `hp`, `mp`, `attack`, `critRate`, `critP`, `defense`, `re`) VALUES
(2, 100, 50, 10, 5, 0.2, 10, 1000),
(3, 100, 50, 10, 5, 0.2, 10, 1000);

-- --------------------------------------------------------

--
-- Structure de la table `inventory`
--

CREATE TABLE `inventory` (
  `inventory_id` double NOT NULL DEFAULT 0,
  `player_id` int(11) NOT NULL DEFAULT 0,
  `item_id` double NOT NULL DEFAULT 0
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Structure de la table `items`
--

CREATE TABLE `items` (
  `item_id` double DEFAULT NULL,
  `name` varchar(50) DEFAULT NULL,
  `statistics` varchar(255) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Structure de la table `skillslearned`
--

CREATE TABLE `skillslearned` (
  `skill_id` double DEFAULT NULL,
  `player_id` int(11) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Structure de la table `users`
--

CREATE TABLE `users` (
  `account_id` bigint(11) NOT NULL,
  `player_id` int(11) NOT NULL,
  `name` varchar(50) NOT NULL DEFAULT '0',
  `class` int(11) DEFAULT 0,
  `currency` float DEFAULT 0,
  `x` double DEFAULT 82,
  `y` double DEFAULT 0.1,
  `z` double DEFAULT 779,
  `rotationx` double NOT NULL DEFAULT 0,
  `rotationy` double NOT NULL DEFAULT 0,
  `rotationz` double NOT NULL DEFAULT 0,
  `region` int(11) NOT NULL DEFAULT 0,
  `level` int(11) NOT NULL DEFAULT 1,
  `exp` double NOT NULL DEFAULT 0,
  `lastEnterWorld` varchar(100) DEFAULT NULL,
  `lastLeaveWorld` varchar(100) DEFAULT NULL,
  `isAlive` tinyint(1) NOT NULL DEFAULT 1
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Déchargement des données de la table `users`
--

INSERT INTO `users` (`account_id`, `player_id`, `name`, `class`, `currency`, `x`, `y`, `z`, `rotationx`, `rotationy`, `rotationz`, `region`, `level`, `exp`, `lastEnterWorld`, `lastLeaveWorld`, `isAlive`) VALUES
(1, 2, 'test', 0, 0, 71.35069, 0.05, 798.4075, 0, 0, 0, 0, 1, 0, NULL, NULL, 1),
(2, 3, 'testo', 0, 0, 82, 0.1, 779, 0, 0, 0, 0, 1, 0, NULL, NULL, 1);

--
-- Index pour les tables déchargées
--

--
-- Index pour la table `accounts`
--
ALTER TABLE `accounts`
  ADD PRIMARY KEY (`account_id`),
  ADD UNIQUE KEY `id` (`account_id`);

--
-- Index pour la table `currentplayerstats`
--
ALTER TABLE `currentplayerstats`
  ADD PRIMARY KEY (`user_id`),
  ADD UNIQUE KEY `user_id` (`user_id`);

--
-- Index pour la table `inventory`
--
ALTER TABLE `inventory`
  ADD PRIMARY KEY (`inventory_id`),
  ADD UNIQUE KEY `id` (`inventory_id`),
  ADD KEY `player_id` (`player_id`);

--
-- Index pour la table `users`
--
ALTER TABLE `users`
  ADD PRIMARY KEY (`player_id`),
  ADD UNIQUE KEY `id` (`player_id`);

--
-- AUTO_INCREMENT pour les tables déchargées
--

--
-- AUTO_INCREMENT pour la table `accounts`
--
ALTER TABLE `accounts`
  MODIFY `account_id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=3;

--
-- AUTO_INCREMENT pour la table `users`
--
ALTER TABLE `users`
  MODIFY `player_id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=4;
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
