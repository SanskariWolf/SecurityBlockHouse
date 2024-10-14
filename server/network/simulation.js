function simulateNetwork() {
    // Simulate the unoptimized network
    const unoptimized = {
      phones: ["Phone1", "Phone2", "Phone3", "Phone4"],
      latency: [120, 130, 150, 160], // Example latencies in ms
      throughput: [10, 12, 9, 11], // Example throughput in Mbps
    };
  
    // Simulate the optimized network with ML and Blockchain
    const optimized = {
      phones: ["Phone1", "Phone2", "Phone3", "Phone4"],
      latency: [60, 55, 65, 70], // Reduced latencies in ms
      throughput: [15, 16, 14, 17], // Increased throughput in Mbps
    };
  
    return { unoptimized, optimized };
  }
  
  module.exports = { simulateNetwork };
  