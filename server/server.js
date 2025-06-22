import express from "express"
import bodyParser from "body-parser";
import fs from "fs"
import "dotenv/config"

const port = 27121
console.log(port);


const app = express();

app.use(bodyParser.json());


const input = "input.txt"
const output = "out.txt"


const inpStream = fs.createWriteStream(input);
const outpStream = fs.createWriteStream(output);

app.post('/', (req, res) => {
  const data = req.body;

  data.tests.forEach((tes) => {
    inpStream.write(tes.input);
    outpStream.write(tes.output);
  })

  console.log("test files updated.")
  res.sendStatus(200);
});

app.listen(port, "0.0.0.0", () => {
  console.log(`Server running on 0.0.0.0:${port}`);
});


