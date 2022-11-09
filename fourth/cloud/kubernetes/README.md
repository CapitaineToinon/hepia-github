## KUBERNETES

[**Antoine Sutter**](mailto:antoine.sutter@hes-so.ch)

# Questions

## 1. What value must be set for this URL?

We need to use `http://api-svc:8081`. The IP address will be resolved for us.

## 2. Use only 1 instance for the Redis-Server. Why?

It's because redis cannot have its data distributed accross multiple instances. Hence if we used multiple instances anyway, we might end up with incoherent data as a different server might answer when the api accesses it.

## 3. What happens if you delete a Frontend or API Pod? How long does it take for the system to react?

It will be re-created virtually instanteniously and the whole website keeps working.

## 4. What happens when you delete the Redis Pod?

It will also be re-created virtually instanteniously but this time the api cannot connect to redis anymore. I assume this is because the api needs to keep an active connection to redis and the current code doesn't support reconnecting to a new server.

## 5. How can you change the number of instances temporarily to 3? Hint: look for scaling in the deployment documentation

The value in the yaml files can either be changed directly or can be overwritten using a the `--replicas` cli argument of the `kubectl scale` command.

```
    --replicas=0:
        The new desired number of replicas. Required.
```

For more info, run `kubectl scale --help`.

## 6. What autoscaling features are available? Which metrics are used?

Using the `kubectl autoscale` command, we can specificy the `--min` and `--max` arguments to specify the amount of pods that can be set by autoscale.

```
    --max=-1:
        The upper limit for the number of pods that can be set by the autoscaler. Required.

    --min=-1:
        The lower limit for the number of pods that can be set by the autoscaler. If it's not specified or negative, the server will apply a default value.
```

For more info, run `kubectl autoscale --help`.

## 7. How can you update a component? (see update in the deployment documentation)

We can update a component using the `kubectl set image` command. For example:

```
kubectl set image deployment/nginx-deployment nginx=nginx:sometag
```

For more info, run `kubectl set image --help`.
