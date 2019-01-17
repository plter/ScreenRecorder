# ScreenRecorder

* [SRV file description](docs/SRV%20File%20Description%20v2.md) 
* [Interfaces between main thread and web worker thread](docs/Interfaces%20between%20main%20thread%20and%20web%20worker%20thread.md).


# Requirements  

1. Download and install nodejs from [https://nodejs.org/en/](https://nodejs.org/en/)  
1. Install cnpm by run command:
    ```shell
    npm install -g cnpm --registry=https://registry.npm.taobao.org
    ```  
1. Download and install docker from [https://www.docker.com/get-started](https://www.docker.com/get-started) (**Note:you have to sign up to docker.com**).


# Build and run  

**Before build**  
**Note: We use `xtiqin/yunpweb:ScreenRecorderBuilderEnv` docker image to build our project.**  
If you are already in this docker repository, you can skip this step, or you can also build the docker image by running `BuildScreenRecorderBuilderEnvDockerImage.sh`  

1. Change work directory to `code`.  
1. Run shell script `BuildAndInstallAndRun.sh` to launch build&run process. 

# Scripts description  

**Note: All scripts are located in code directory.**

| Script | Description |
| --- | --- |
| `BuildAndInstallAndRun.sh` | Build to `out/debug` directory, then install node dependencies via cnpm, then run ScreenRecorder |
| `BuildAndRun.sh` | Build to `out/debug` directory ,then run ScreenRecorder, if you had installed node dependencies , you can just run this script |
| `Run.sh` | Run ScreenRecorder in `out/debug` directory without rebuild | |