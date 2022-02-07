import express from 'express';
const app = express();
app.use(express.json());

app.get('/api/tasks', (req, res) => {

});

export default app;