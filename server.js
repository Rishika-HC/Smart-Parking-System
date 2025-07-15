const express = require('express');
const mongoose = require('mongoose');
const bodyParser = require('body-parser');
const cors = require('cors');

const app = express();
app.use(cors());
app.use(bodyParser.json());

mongoose.connect('mongodb://localhost:27017/parkingDB', {
  useNewUrlParser: true,
  useUnifiedTopology: true,
});

const ParkingSchema = new mongoose.Schema({
  slot1: String,
  slot2: String,
  timestamp: { type: Date, default: Date.now },
});

const Parking = mongoose.model('Parking', ParkingSchema);

app.post('/api/parking', async (req, res) => {
  try {
    const { slot1, slot2 } = req.body;
    const newEntry = new Parking({ slot1, slot2 });
    await newEntry.save();
    res.status(200).json({ message: 'Data saved successfully' });
  } catch (error) {
    res.status(500).json({ error: 'Error saving data' });
  }
});

const PORT = 3000;
app.listen(PORT, () => {
  console.log(`🚀 Server running at http://localhost:${PORT}`);
});
