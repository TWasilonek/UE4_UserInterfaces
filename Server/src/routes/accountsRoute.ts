import express from 'express';
import * as Accounts from '../db/accountsDb';

const router = express.Router();

router.post('/login', (req, res) => {
    const { data } = req.body;
    const { username, password } = data;
    
    if (!username || !password) {
        res.status(401).send("Invalid data");
    }

    try {
        const success = Accounts.login(username, password)
        if (!success) {
            res.status(401);
        }
        res.status(200);
    } catch (e) {
        res.status(500).send('Something went wrong. Try again.\n');
    }
});

router.post('/signup', (req, res) => {
    // TODO: 
    res.status(500).send('Something went wrong. Try again.\n')
});

export default router;