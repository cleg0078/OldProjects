# Base image
FROM node:17 

# Set working directory to /opt
WORKDIR /opt

# Install dependencies for the React app
#TODO check for a replacement using npx then replace CMD by CMD ["npx", "serve", "-s", "build"]
COPY apps/main/package*.json /opt/apps/
RUN cd /opt/apps && npm install && npm install -g serve

# Install dependencies for the API
COPY api/package*.json /opt/api/
RUN cd /opt/api && npm install

# Copy both the API and apps code to /opt
COPY . /opt

# Build the React app
RUN cd /opt/apps/main && npm run build

# Start both the API and the React app
CMD ["sh", "-c", "node /opt/api/server.js & serve -s /opt/apps/main/build -l 3000"]
