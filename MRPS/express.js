import express from 'express';
import crypto from 'crypto';
// import {DB} from './database/index.js';
// import {redis} from './database/redis.js';
// import {generateCode , getMaxId} from './utils.js';


const app=express();
process.title="node-express";
app.use(express.json({limit:"1mb"}));

app.get('/simple',(req,res)=>{
    res.json({message:"hello to 1MRPS"});
})






app.listen(3000,()=>{
    console.log("server is running on port 3000");
});