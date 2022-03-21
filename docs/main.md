# Paxosme 

## Paxos Player & Main pipeline

### Players

- `Proposer` 
  - **Pi** denotes one proposer
  - **P*** denotes all proposers

- `Acceptor` 
  - **Ai** denotes one acceptor
  - **A*** denotes all acceptors
  
- `Learner`
  - `Following` **L.Fi** denotes following learner
  - `Leading` **L.Li** denotes following learner


### Msg Types

- `kACCEPTOR_PROPOSE_BROADCAST` 

  **Pi** --> **A***
  
  Timeout
  
  ACK


- `kACCEPTOR_ACCEPT_BROADCAST`

  **Pi** --> **A***

  Timeout

  ACK

- `kPROPOSER_PROPOSE_ACK`

  **Ai** --> **Pi**

- `kPROPOSER_ACCEPT_ACK`

  **Ai** --> **Pi**

- `kLEARNER_SENDER_PUBLISH_CHOSEN_VALUE`

  **Pi** --> **Ai**

  **Pi** --> **L.F**

  


