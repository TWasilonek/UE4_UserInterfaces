import server from './src/app';

const port = 3000;

server.listen(port, () => {
    console.log(`UE4 Interfaces server listening at http://localhost:${port}`);
});
