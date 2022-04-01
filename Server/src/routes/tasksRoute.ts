import express from 'express';
import * as Tasks from '../db/tasksDb';

const router = express.Router();

// Get tasks
router.get('/', (req, res) => {
    try {
        const tasks = Tasks.getTasks();
        res.status(200).json({ tasks });
    } catch (e) {
        res.status(500).send('Something went wrong. Try again.\n');
    }
});

// Crete task
router.post('/', (req, res) => {
    try {
        const { data } = req.body; 
        const newTask = Tasks.createTask(data);
        console.log('Task crated: ', JSON.stringify(newTask));
        res.status(200).json({ task: newTask });
    } catch (e) {
        res.status(500).send('Something went wrong. Try again.\n');
    }
});

// Update task
router.put('/:taskId', (req, res) => {
    try {
        const { taskId } = req.params;
        const { data } = req.body;
        const updatedTask = Tasks.updateTask(taskId, data);
        console.log('Task updated: ', JSON.stringify(updatedTask));
        res.status(200).json({ task: updatedTask });
    } catch (e) {
        res.status(500).send('Something went wrong. Try again.\n');
    }
});

// Delete task
// UE4 HTTP module does not support the DELETE verb
router.post('/:taskId/delete', (req, res) => {
    try {
        const { taskId } = req.params;
        Tasks.deleteTask(taskId);
        console.log('Task deleted');
        res.sendStatus(200);
    } catch (e) {
        res.status(500).send('Something went wrong. Try again.\n');
    }
});

export default router;