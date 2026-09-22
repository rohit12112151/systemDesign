import cpeak ,{parseJSON} from 'cpeak';
import crypto from 'crypto';

const app=cpeak();
process.title="node-cpeak";
app.beforeEach(parseJSON({limit:1024*1024}));
// app.use(express.json({limit:"1mb"}));

app.route("get",'/simple',(request,response)=>{
    return response.json({message:"hello to 1MRPS"});
});


app.route("patch", `/update-something/:id/:name`, (req, res) => {
  const { id, name } = req.params;
  const { value1, value2 } = req.query;

  // Validate id and name
  if (isNaN(Number(id))) {
    return res.status(400).json({ error: "id must be a number" });
  } else if (!name || name.length < 3) {
    return res
      .status(400)
      .json({ error: "name is required and must be at least 3 characters" });
  }

  const formattedFooValues = [];

  for (let i = 1; i <= 10; i++) {
    const val = req.body[`foo${i}`];
    const formattedVal = typeof val === "string" ? `${val}. ` : val;
    formattedFooValues.push(formattedVal);
  }

  // Adding all the formatted foo values together
  const totalFoo = formattedFooValues.join("");

  // Generating a few kilobytes of dummy data
  const dummyHistory = Array.from({ length: 100 }).map((_, i) => ({
    event_id: Number(id) + i,
    timestamp: new Date().toISOString(),
    action: `Action performed by ${name}`,
    metadata:
      "This is a string intended to take up space to simulate a medium-sized production API response object.".repeat(
        2,
      ),
    status: i % 2 === 0 ? "success" : "pending",
  }));

  res.json({
    id,
    name,
    value1,
    value2,
    total_foo: String(totalFoo).toUpperCase(),
    history: dummyHistory,
  });
});






app.listen(3002, () => {
  console.log("Server running at http://localhost:3002");
});