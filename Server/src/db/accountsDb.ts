import { v4 as uuidv4 } from 'uuid';

export interface PlayerAccount {
    username: string;
    password: string;
    id: string;
}

let accounts: PlayerAccount[] = [
    { id: uuidv4(), username: "master1", password: "test" },
];

export const login = (username: string, password: string) => {
    const found = accounts.find((acc) => acc.username === username && acc.password === password);
    if (!found) {
        console.error('Account data not found');
        return false;
    }

    return true;
};

export const signup = (username: string, password: string) => {
    const found = accounts.find((acc) => acc.username === username && acc.password === password);
    if (found) {
        console.error('Account already exists');
        return false;
    }

    const newAccount = {
        username,
        password,
        id: uuidv4(),
    };

    accounts.push(newAccount);

    return true;
};
