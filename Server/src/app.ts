import express from 'express';
import tasksRouter from './routes/tasksRoute';
import authRouter from './routes/authRouter';

const app = express();
app.use(express.json());

app.use('/api/tasks', tasksRouter);
app.use('/api/auth', authRouter);

export default app;