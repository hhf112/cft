import express from "express"
import bodyParser from "body-parser";
import fs from "fs"
import "dotenv/config"

const port = process.env.PORT;

const app = express();

app.use(bodyParser.json());


// if (process.argv.length != 4) {
//   console.log("wrong usage.")
//   process.exit(1)
// }
//
// const [input, output] = [process[2], process[3]]
//
const input = "input.txt"
const output = "out.txt"


const inpStream = fs.createWriteStream(input);
const outpStream = fs.createWriteStream(output);

app.post('/', (req, res) => {
  const data = req.body;

  data.tests.forEach((tes) => {
    inpStream.write(tes.input);
    inpStream.write(tes.output);
  })

  console.log("test files updated.")
  res.sendStatus(200);
});

app.listen(port, err => {
  if (err) {
    console.error(err);
    process.exit(1);
  }

  console.log(`Listening on port ${port}`);
});


