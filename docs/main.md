# Paxosme 

## Paxos Player & MA.in P.ipeline

### Players

- `Proposer` 
  - **P.i** denotes one proposer
  - **P.*** denotes all proposers


- `Acceptor` 
  - **A.i** denotes one acceptor
  - **A.*** denotes all acceptors
  

- `Learner`
  - **L.i** denotes one learner
  - **L.*** denotes all learners
  - **LF.i** denotes following learner
  - **LF.*** denotes following learner


### Msg Types

#### Event Messages

- `kMSG_PROPOSE_BROADCAST`

  - Players
    - **P.i** --> **A.***

  - Timeout handle 
  
  - ACK
    - kMSG_PROPOSE_ACK


- `kMSG_ACCEPT_BROADCAST`

  - Players
    - **P.i** --> **A.***

  - Requires
    - Timeout handle
    - ACK msg


- `kMSG_PROPOSE_ACK`

  - Players
     - **A.i** --> **P.j**


- `kMSG_ACCEPT_ACK`
  
  - Players
    - **A.i** --> **P.j**


- `kMSG_VALUE_CHOSEN`
  
  - Players
    - **P.i** --> **L.i**
  

- `kMSG_VALUE_CHOSEN_BROADCAST`
  
  - Players
      - **L.i** --> **L.***

#### Schedule Messages

- `kMSG_SHALL_I_LEARN`
  
  - Players
    - **L.i** --> **L.***

  - ACK
    - `kMSG_TELL_INSTANCE_ID`


- `kMSG_TELL_INSTANCE_ID`

  - Players
    - **L.i** --> **L.***

  - ACK
    - `kMSG_CONFIRM_LEARN`


