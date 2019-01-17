# Interfaces between main thread and web worker thread 

## Message format 

A message is with json format.   
For example:  
```json
{"type":"ready"}
``` 


# SRVReaderTask Worker 

## Post messages 

| Command | Description |
| --- | --- |
| ready | Work ready |