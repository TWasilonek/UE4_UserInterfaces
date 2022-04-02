import express from 'express';
import * as Accounts from '../db/accountsDb';

const router = express.Router();

router.post('/login', (req, res) => {
    const { username, password  } = req.body;
    
    if (!username || !password) {
        res.sendStatus(401);
    }

    try {
        const success = Accounts.login(username, password);
        if (!success) {
            res.sendStatus(401);
        }
        res.sendStatus(200);
    } catch (e) {
        res.sendStatus(500);
    }
});

router.post('/signup', (req, res) => {
    // TODO: 
    res.sendStatus(500);
});

export default router;