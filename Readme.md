# IPK Projekt 1

---

## Author

---

Name: Juraj Dedič
Login: xdedic07

## Implementation

---

### 1. Domain name

---

Domain name is read by opening and reading `/proc/sys/kernel/hostname` file

### 2. CPU Info

---

CPU Info is stored in `/proc/cpuinfo`.
I solved this using `popen()` call.
To open and pipe the text `cat` is called first.
Then I used `awk` to extract the line and column containing the CPU model.

### 3. CPU Load

---

The current load can be read from `/proc/stat`.
We need to read the file 2 times.
Then we calculate how much time was the CPU idling in those 2 reads separately.
Consequently we determine the difference in the idle and total times
We then calculate the CPU load based on the differences in the total and idle times.

### 4. The server

---

#### 4.0 Prepare

We check whether the port number was correctly specified.

#### 4.1 Setup

We have buffer for storing the request
We use function `socket()` to create the server socket. `<!-- `setsockopt` to set  -->`
Structures for server and client addresses are then specified.

#### 4.2 Connection

When connection is established we copy the request into the buffer
After extracting the path we check whether the path is correct and decide the right reponse.
If the request method isn't **GET** or the path is wrong, we send response `400 Bad request`.
For paths `/hostname` and `cpu-name` we send back the string which is saved in a variable.
When requesting `/load`, the `get_cpu_load()` function is called.
In order to respond in the correct way, response prefix is appended to the string.
Then the response is sent using `write()` and the connection closes.
