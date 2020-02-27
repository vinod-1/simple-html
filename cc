version: 2
jobs:
  publish:
    docker:
      - image: circleci/openjdk:8u222-jdk-stretch
    working_directory: ~/repo
    steps:
      - checkout
      - run: ./gradlew clean fatJar
      - run: ls build/libs
      - run:
          name: Installing AWS CLI
          command: |
            sudo apt-get -y -qq update
            sudo apt-get install -y awscli
            sudo apt-get install -y python-pip python-dev build-essential
            sudo pip install awsebcli --upgrade
      - run:
          name: Deploying Jar to S3
          command: |
            aws s3 sync build/libs/ s3://maven-lib --region ap-south-1
            
workflows:
  version: 2
  build-and-publish:
    jobs:
      - Approve:
          type: approval
 #         filters:
 #           branches:
 #             only: develop
      - publish:
          requires:
            - Approve
          #type: approval
#          filters:
#            branches:
#              only: develop
