import { v4 as uuidv4 } from 'uuid';

export interface Task {
    text: string;
    completed: boolean;
    id: string;
}

const tasks: Task[] = [
    { id: uuidv4(), text: 'Create tasks', completed: true },
    { id: uuidv4(), text: 'Add HTTP module to Unreal Engine', completed: false },
    { id: uuidv4(), text: 'Wire up Unreal Engine HTTP actions', completed: false },
];

export const getTasks = () => {
    return tasks;
};
