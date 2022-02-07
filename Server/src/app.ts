import express from 'express';
import * as Tasks from './tasks';

const app = express();
app.use(express.json());

app.get('/api/tasks', (req, res) => {
    try {
        const tasks = Tasks.getTasks();
        res.status(200).json({ tasks });
    } catch (e) {
        res.status(500).send('Something went wrong. Try again.\n');
    }
});

export default app;