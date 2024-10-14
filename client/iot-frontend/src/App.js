import React, { useState, useEffect } from "react";
import { NetworkGraph } from "./components/NetworkGraph"; // Visualization of network
import "./App.css"; // Optional styles

function App() {
  const [networkData, setNetworkData] = useState(null);
  const [loading, setLoading] = useState(true);

  // Fetch data for both networks (with and without optimization)
  useEffect(() => {
    fetch("/api/get-network-data")
      .then((res) => res.json())
      .then((data) => {
        setNetworkData(data);
        setLoading(false);
      });
  }, []);

  if (loading) return <div>Loading Network Simulation...</div>;

  return (
    <div className="App">
      <h1>Network Simulation</h1>
      <h2>Unoptimized Network</h2>
      <NetworkGraph data={networkData.unoptimized} />

      <h2>Optimized Network (ML + Blockchain)</h2>
      <NetworkGraph data={networkData.optimized} />
    </div>
  );
}

export default App;
