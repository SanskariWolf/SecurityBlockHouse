const crypto = require("crypto");

class Block {
  constructor(index, data, previousHash) {
    this.index = index;
    this.timestamp = Date.now();
    this.data = data;
    this.previousHash = previousHash;
    this.hash = this.calculateHash();
  }

  calculateHash() {
    return crypto
      .createHash("sha256")
      .update(this.index + this.timestamp + this.data + this.previousHash)
      .digest("hex");
  }
}

class Blockchain {
  constructor() {
    this.chain = [this.createGenesisBlock()];
  }

  createGenesisBlock() {
    return new Block(0, "Genesis Block", "0");
  }

  addBlock(newBlock) {
    newBlock.previousHash = this.chain[this.chain.length - 1].hash;
    newBlock.hash = newBlock.calculateHash();
    this.chain.push(newBlock);
  }
}

const blockchain = new Blockchain();

module.exports = { blockchain };
