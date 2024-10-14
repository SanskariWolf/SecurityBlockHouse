export const fetchNetworkData = async () => {
    const response = await fetch("/api/get-network-data");
    return response.json();
  };
  