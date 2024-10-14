const express = require("express");
const { simulateNetwork } = require("./network/simulation");
const { blockchain } = require("./blockchain/blockchain");

const app = express();
const PORT = process.env.PORT || 5000;

// API endpoint to provide network data
app.get("/api/get-network-data", (req, res) => {
  const networkData = simulateNetwork();
  res.json(networkData);
});

// Start server
app.listen(PORT, () => {
  console.log(`Server is running on port ${PORT}`);
});
