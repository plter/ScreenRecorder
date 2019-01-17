# SRV Description

Version: 1.0 

## Fields

| Description | bytes | type |
| --- | --- | --- |
| Package type | 1 | uint8 |
| Package name length | 4 | uint32 |
| Package name | Result value of "Package name length" you read | String |
| md5 verify code | 32 | String (Fixed length) |
| Package content length | 4 | uint32 |
| Package content | The result of "Package content length" you read | bytes |

## Package types reference

| Value | Description |
| --- | --- |
| 1 | webm file parts |
| 2 | webm file header |
| 3 | webm file footer |
| 4 | config file |

# Data types reference 

## webm header 

| Length(in bytes) | Value | Type | Description |
| --- | --- | --- | --- |
| 1 | 2 | uint8 | Data package type |
| 4 | Result of reading | uint32 | File name length in bytes |
| File name length read | Result of reading | String | webm file name |

## webm file footer

| Length | Value | Type | Description |
| --- | --- | --- | --- |
| 1 | 3 | uint8 | Package type |

## webm part

| Length | Value | Type | Description |
| --- | --- | --- | --- |
| 1 | 1 | uint8 | Package type |
| 4 | Result of reading | uint32 | webm part data length |
| Part data length read | Result of reading | binary | webm part data |

## config file

**Note ：Config file is a json format file,and it contains all the srv file information。**  

| Length | Value | Type | Description |
| --- | --- | --- | --- |
| 1 | 4 | uint8 | Package type |
| 4 | Result of reading | uint32 | config file content length |
| Result of reading | Result of reading | binary | config file content |
