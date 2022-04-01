import { v4 as uuidv4 } from 'uuid';

export interface Task {
    text: string;
    completed: boolean;
    id: string;
}

let tasks: Task[] = [
    { id: uuidv4(), text: 'Create tasks', completed: true },
    { id: uuidv4(), text: 'Add HTTP module to Unreal Engine', completed: false },
    { id: uuidv4(), text: 'Wire up Unreal Engine HTTP actions', completed: false },
];

export const getTasks = () : Task[] => {
    return tasks;
};

export const getTaskById = (id: string): Task => {
    const foundTask = tasks.find((task) => task.id === id);
    if (!foundTask) {
        throw Error('Task not found');
    }

    return foundTask;
}

export const createTask = (data: Partial<Task>) : Task => {
    const newTask = {
        text: data.text || '',
        completed: false,
        id: uuidv4(),
    };

    tasks.push(newTask);
    return newTask;
}

export const updateTask = (id: string, data: Task) : Task => {
    const foundTask = tasks.find((task) => task.id === id);
    if (!foundTask) {
        throw Error('Task not found');
    }

    const updatedTask = {
        ...foundTask,
        text: data.text,
        completed: data.completed,
    };

    tasks = tasks.map(task => {
        if (task.id === id) {
            return updatedTask;
        }
        return task;
    });

    return updatedTask;
}

export const deleteTask = (id: string) => {
    const foundTask = tasks.find((task) => task.id === id);
    if (!foundTask) {
        throw Error('Task not found');
    } 

    tasks = tasks.filter(task => task.id !== id);
}