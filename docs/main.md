
# Paxosme 

## Paxos Player & Main Pipeline

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

  - Players **P.i** --> **A.***

  - Timeout handle 
  
  - ACK `kMSG_PROPOSE_ACK`


- `kMSG_ACCEPT_BROADCAST`

  - Players **P.i** --> **A.***

  - Timeout handle
  
  - ACK `kMSG_ACCEPT_ACK`


- `kMSG_PROPOSE_ACK`

  - Players **A.i** --> **P.j**


- `kMSG_ACCEPT_ACK`
  
  - Players **A.i** --> **P.j**


- `kMSG_VALUE_CHOSEN`
  
  - Players **P.i** --> **L.i**
  

- `kMSG_VALUE_CHOSEN_BROADCAST`
  
  - Players **L.i** --> **L.***


- `kMSG_TELL_INSTANCE_ID`

  - Players **L.i** --> **L.j**

  - ACK `kMSG_CONFIRM_LEARN`


- `kMSG_CONFIRM_LEARN`

  - Players **L.i** --> **L.j**


- `kMSG_LEARNER_VALUE_SEND`
  
  - Players **L.i** --> **L.j**


- `kMSG_LEARNER_VALUE_SYNC`

  - Players **L.i** --> **L.j**
  - ACK `kMSG_SYNC_VALUE_ACK`


- `kMSG_SYNC_VALUE_ACK`

  - Players **L.i** --> **L.j**

#### Schedule Messages

- `kMSG_SHALL_I_LEARN`
  
  - Players **L.i** --> **L.***

  - ACK `kMSG_LEARNER_VALUE_SEND` `kMSG_TELL_INSTANCE_ID`



### Event Types

#### Timeout Events

- `kEVENT_PROPOSE_TIMEOUT`
  
  - Proposer
  - Callback `Proposer.ProposeTimeoutCallback`


- `kEVENT_ACCEPT_TIMEOUT`

  - Proposer
  - Callback `Proposer.ProposeTimeoutCallback`


- `kEVENT_NEWVALUE_TIMEOUT`

  - Proposer
  - Callback `Proposer.NewValueTimeoutCallback`


#### Schedule Events

- `kEVENT_SHALL_I_LEARN`

  - Learner
  - Callback `Learner.ShallLearn`