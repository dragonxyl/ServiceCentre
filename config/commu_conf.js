{
    "dispatchers": [
        {
            "index": 0,
            "routers": [
                {
                    "key": "",
                    "service": "UserDefaultService"
                },
                {
                    "key": "task",
                    "service": "TaskService"
                },
                {
                    "key": "algorithm",
                    "service": "AlgorithmService"
                },
                {
                    "key": "images",
                    "service": "FileService"
                }
            ]
        },
        {
            "index": 1,
            "routers": [
                {
                    "key": "",
                    "service": "UserDefaultServiceT"
                },
                {
                    "key": "task",
                    "service": "TaskServiceT"
                },
                {
                    "key": "algorithm",
                    "service": "AlgorithmServiceT"
                }
            ]
        }
    ],
    "receivers": [
        {
            "port": 20855,
            "peer_response_timeout": 20000,
            "dispatchers_index": [
                0
            ]
        },
        {
            "port": 20866,
            "peer_response_timeout": 20000,
            "dispatchers_index": [
                1
            ]
        }
    ]
}