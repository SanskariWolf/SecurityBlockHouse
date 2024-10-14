import React from "react";
import { Line } from "react-chartjs-2"; // Or use D3.js

export const NetworkGraph = ({ data }) => {
  const chartData = {
    labels: data.phones,
    datasets: [
      {
        label: "Latency (ms)",
        data: data.latency,
        borderColor: "rgb(75, 192, 192)",
        tension: 0.1,
      },
      {
        label: "Throughput (Mbps)",
        data: data.throughput,
        borderColor: "rgb(153, 102, 255)",
        tension: 0.1,
      },
    ],
  };

  return <Line data={chartData} />;
};
