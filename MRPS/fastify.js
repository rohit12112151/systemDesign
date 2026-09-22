import Fastify from 'fastify';
import crypto from 'crypto';

const app=Fastify({bodyLimit:1024*1024});
process.title="node-fastify";
// app.use(express.json({limit:"1mb"}));

app.get('/simple',(request,reply)=>{
    reply.send({message:"hello to 1MRPS"});
});






app.listen({ port: 3001 }, (error, address) => {
  if (error) {
    console.error(error);
    process.exit(1);
  }

  console.log(`Server running at ${address}`);
});